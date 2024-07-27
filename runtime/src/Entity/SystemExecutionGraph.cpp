#include "RedEngine/Entity/SystemExecutionGraph.hpp"

#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/SystemQuery.hpp"

namespace red
{
SystemExecutionNode::SystemExecutionNode(StringView name, ExecutionGraph* graph) : ExecutionNode(name, graph)
{
}

SystemExecutionGraph::SystemExecutionGraph(World* world) : m_world(world)
{
}

ExecutionNode& SystemExecutionGraph::GetOrCreateNode(StringView name)
{
    for (auto* node : m_nodes)
    {
        if (node->GetName() == name)
            return *node;
    }

    m_nodes.push_back(red_new(SystemExecutionNode, name, this));

    m_bDirty = true;

    return *m_nodes.back();
}

bool SystemExecutionGraph::CanBePlacedInside(ExecutionBucket& bucket, const ExecutionNode& node)
{
    if (!ExecutionGraph::CanBePlacedInside(bucket, node))
    {
        return false;
    }

    SystemExecutionNode& sysNode = (SystemExecutionNode&)node;
    auto* baseQuery = sysNode.m_system->GetQuery();

    for (auto* otherNode : bucket.m_nodes)
    {
        SystemExecutionNode* otherSysNode = (SystemExecutionNode*)otherNode;
        const bool canBuRunInParrallele = baseQuery->CanBeRunInParalleleWith(otherSysNode->m_system->GetQuery());

        if (!canBuRunInParrallele)
        {
            return false;
        }
    }

    return true;
}
}  // namespace red