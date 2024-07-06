#include "RedEngine/Thread/ExecutionGraph.hpp"

#include "RedEngine/Core/Debug/DebugMacros.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"

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

void ExecutionGraph::Execute()
{
    if (m_bDirty)
    {
        m_sortedNodes.clear();
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
                break;
            }
        }
        m_bDirty = false;
    }

    for (int i = 0; i < m_sortedNodes.size(); i++)
    {
        auto* node = m_sortedNodes[i];
        node->m_func();
    }
}
}  // namespace red