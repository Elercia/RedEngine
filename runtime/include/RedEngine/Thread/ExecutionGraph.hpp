#pragma once

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"

namespace red
{
class ExecutionNode
{
    friend class ExecutionGraph;

public:
    using Func = std::function<void()>;

    ExecutionNode(StringView name, ExecutionGraph* graph);

    ExecutionNode& After(StringView name);
    ExecutionNode& Before(StringView name);

private:
    ExecutionGraph* m_owner;
    String m_name;
    Array<ExecutionNode*> m_befores;
    ExecutionNode::Func m_func;
    bool m_created;

    
    bool m_permanentMark;
    bool m_tempMark;
};

class ExecutionGraph : public Uncopyable
{
    friend class ExecutionNode;

public:
    ExecutionGraph();
    ~ExecutionGraph();

    ExecutionNode& Node(StringView name, ExecutionNode::Func func);

    void Execute();

private:
    ExecutionNode& GetOrCreateNode(StringView name);
    bool Visit(ExecutionNode& node);

    Array<ExecutionNode*> m_nodes;
    Array<ExecutionNode*> m_sortedNodes;
    bool m_bDirty;
};
}  // namespace red