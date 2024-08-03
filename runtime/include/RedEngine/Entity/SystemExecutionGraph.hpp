#pragma once

#include "RedEngine/Thread/ExecutionGraph.hpp"
#include "RedEngine/Entity/World.hpp"

namespace red
{
class SystemExecutionNode : public ExecutionNode
{
public:
    SystemExecutionNode(StringView name, ExecutionGraph* graph);

    template <typename Sys>
    SystemExecutionNode& After();
    template <typename Sys>
    SystemExecutionNode& Before();

    BaseSystem* m_system;
};

class SystemExecutionGraph : public ExecutionGraph
{
public:
    SystemExecutionGraph(World* world);

    template <typename Sys>
    SystemExecutionNode& AddSystem();

    virtual ExecutionNode& GetOrCreateNode(StringView name) override;
    virtual bool CanBePlacedInside(ExecutionBucket& bucket, const ExecutionNode& node) override;

    using ExecutionGraph::Process;
    using ExecutionGraph::Execute;
    using ExecutionGraph::GetBuckets;

private:
    World* m_world;
};
}  // namespace red

#include "inl/SystemExecutionGraph.inl"
