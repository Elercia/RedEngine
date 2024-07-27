
namespace red
{
template <typename Sys>
SystemExecutionNode& SystemExecutionNode::After()
{
    constexpr auto typeInfp = TypeInfo<Sys>();
    ExecutionNode::After(typeInfp.name);
    return *this;
}

template <typename Sys>
SystemExecutionNode& SystemExecutionNode::Before()
{
    constexpr auto typeInfp = TypeInfo<Sys>();
    ExecutionNode::Before(typeInfp.name);
    return *this;
}

template <typename Sys>
inline SystemExecutionNode& SystemExecutionGraph::AddSystem()
{
    constexpr auto typeInfo = TypeInfo<Sys>();

    auto* system = m_world->GetSystem<Sys>();
    RedAssert(system != nullptr);

    auto& node = (SystemExecutionNode&)Node(typeInfo.name, [=]() { system->Update(); });
    node.m_system = system;

    return node;
}
}  // namespace red
