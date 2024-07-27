#pragma once

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"
#include "RedEngine/Thread/Thread.hpp"

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

    bool DependsOnRecursive(const ExecutionNode* other) const;

    const String& GetName() const;

protected:
    ExecutionGraph* m_owner;
    String m_name;
    Array<ExecutionNode*> m_befores;
    ExecutionNode::Func m_func;
    bool m_created;

    
    bool m_permanentMark;
    bool m_tempMark;
};

struct ExecutionBucket
{
    Array<ExecutionNode*> m_nodes;
};

class ExecutionGraph : public Uncopyable
{
    friend class ExecutionNode;

public:
    ExecutionGraph();
    ~ExecutionGraph();

    ExecutionNode& Node(StringView name, ExecutionNode::Func func);

    void Process();
    void Execute();

    const Array<ExecutionBucket>& GetBuckets() const;

protected:
    virtual ExecutionNode& GetOrCreateNode(StringView name);
    bool Visit(ExecutionNode& node);

    virtual bool CanBePlacedInside(ExecutionBucket& bucket, const ExecutionNode& node);

    Array<ExecutionNode*> m_nodes;
    Array<ExecutionNode*> m_sortedNodes; // TODO add generic templated function to sort task graphs
    Array<ExecutionBucket> m_buckets; 
    bool m_bDirty;
};
}  // namespace red