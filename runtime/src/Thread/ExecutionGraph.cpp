#include "RedEngine/Thread/ExecutionGraph.hpp"

#include "RedEngine/Core/Debug/DebugMacros.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Engine.hpp"

namespace red
{
ExecutionNode::ExecutionNode(StringView name, ExecutionGraph* graph)
    : m_owner(graph), m_name(name), m_befores(), m_created(false), m_permanentMark(false), m_tempMark(false)
{
}

ExecutionNode& ExecutionNode::After(StringView name)
{
    m_owner->m_bDirty = true;

    m_befores.push_back(&m_owner->GetOrCreateNode(name));

    return *this;
}

ExecutionNode& ExecutionNode::Before(StringView name)
{
    m_owner->m_bDirty = true;

    m_owner->GetOrCreateNode(name).m_befores.push_back(this);

    return *this;
}

bool ExecutionNode::DependsOnRecursive(const ExecutionNode* other) const
{
    RedAssert(other != this);

    if (m_befores.FindIf([&](ExecutionNode* arOther) { return arOther == other; }) != Array<ExecutionNode*>::npos)
    {
        return true;
    }

    for (auto* beforeNode : m_befores)
    {
        if (beforeNode->DependsOnRecursive(other))
        {
            return true;
        }
    }

    return false;
}



const String& ExecutionNode::GetName() const
{
    return m_name;
}

ExecutionGraph::ExecutionGraph() : m_bDirty(true)
{
}

ExecutionGraph::~ExecutionGraph()
{
    for (auto* node : m_nodes)
    {
        red_delete(node);
    }
}

ExecutionNode& ExecutionGraph::Node(StringView name, ExecutionNode::Func func)
{
    auto& node = GetOrCreateNode(name);

    RedAssert(node.m_created == false);

    node.m_func = func;
    node.m_created = true;

    return node;
}

ExecutionNode& ExecutionGraph::GetOrCreateNode(StringView name)
{
    for (auto* node : m_nodes)
    {
        if (node->m_name == name)
            return *node;
    }

    m_nodes.push_back(red_new(ExecutionNode, name, this));

    m_bDirty = true;

    return *m_nodes.back();
}

bool ExecutionGraph::CanBePlacedInside(ExecutionBucket& bucket, const ExecutionNode& node)
{
    for (auto* bucketNode : bucket.m_nodes)
    {
        if (node.DependsOnRecursive(bucketNode))
        {
            return false;
        }
    }

    return true;
}

const Array<ExecutionBucket>& ExecutionGraph::GetBuckets() const
{
    return m_buckets;
}

bool ExecutionGraph::Visit(ExecutionNode& node)
{
    if (node.m_permanentMark)
        return true;

    if (node.m_tempMark)
        return false;

    node.m_tempMark = true;

    for (auto other : node.m_befores)
    {
        Visit(*other);
    }

    node.m_tempMark = false;
    node.m_permanentMark = true;

    m_sortedNodes.push_back(&node);

    return true;
}

void ExecutionGraph::Process()
{
    // Compute the list of sorted node using topological sorting
    m_sortedNodes.clear();
    m_buckets.clear();
    for (auto* node : m_nodes)
    {
        node->m_permanentMark = false;
        node->m_tempMark = false;
    }

    for (auto* node : m_nodes)
    {
        if (node->m_permanentMark == false && Visit(*node) == false)
        {
            RED_LOG_ERROR("Circular dependency inside this execution graph");
            return;
        }
    }

    // Construct a list of buckets
    // Each bucket will be runned in parrallel
    for (auto* node : m_sortedNodes)
    {
        bool hasBeenPlaced = false;

        // Check if the node can be placed inside a bucket
        // It can be placed inside a bucket if the bucket does not contain a node that is in the current node dependency
        // (recursivelly)
        for (auto& bucket : m_buckets)
        {
            if (CanBePlacedInside(bucket, *node))
            {
                hasBeenPlaced = true;
                bucket.m_nodes.push_back(node);
                break;
            }
        }

        if (!hasBeenPlaced)
        {
            auto& bucket = m_buckets.emplace_back();

            bucket.m_nodes.push_back(node);
        }
    }

    m_bDirty = false;
}

void ExecutionGraph::Execute()
{
    if (m_bDirty)
    {
        Process();

        if (m_bDirty)
            return;  // Illegal graph
    }

    auto& scheduler = Engine::GetInstance()->GetScheduler();

    for (uint32 i = 0; i < m_buckets.size(); i++)
    {
        auto* bucket = &m_buckets[i];
        RedAssert(bucket->m_nodes.empty() == false);

        WaitGroup wg(bucket->m_nodes.size());

        for (auto* node : bucket->m_nodes)
        {
            scheduler.Schedule(
                [=]() // By copy since it will be dispatched in multithread
                {
                    node->m_func();
                    wg.Done();
                });
        }

        wg.Wait();
    }
}
}  // namespace red