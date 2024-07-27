#include "RedEngine/Core/Engine.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/SystemExecutionGraph.hpp"
#include "RedEngine/Entity/Transform.hpp"
#include "RedEngine/Entity/World.hpp"
#include "RedEngine/Thread/ExecutionGraph.hpp"

#include <catch2/catch.hpp>

#include "EngineTest.hpp"

using namespace red;

TEST_CASE("Simple execution graph", "[Thread]")
{
    auto engine = CreateEngineFrom<EngineTest>(0, nullptr);

    ExecutionGraph graph;

    int counter = 0;
    int a = 0;
    int b = 0;
    int c = 0;

    graph.Node("B", [&]() { b = counter++; }).After("A").Before("C");
    graph.Node("A", [&]() { a = counter++; });
    graph.Node("C", [&]() { c = counter++; });

    graph.Execute();

    REQUIRE(a == 0);
    REQUIRE(b == 1);
    REQUIRE(c == 2);

    engine->Destroy();
}

TEST_CASE("Parallel graph", "[Thread]")
{
    auto engine = CreateEngineFrom<EngineTest>(0, nullptr);

    ExecutionGraph graph;

    std::atomic<int> counter = 0;
    int a, b, c, d, e, f, g;
    a = b = c = d = e = f = g = 0;

    auto& nodeA = graph.Node("A", [&]() { a = counter++; });

    auto& nodeB = graph.Node("B", [&]() { b = counter++; }).After("A");

    auto& nodeC = graph.Node("C", [&]() { c = counter++; }).After("B");
    auto& nodeD = graph.Node("D", [&]() { d = counter++; }).After("B");
    auto& nodeE = graph.Node("E", [&]() { e = counter++; }).After("B");

    auto& nodeF = graph.Node("F", [&]() { f = counter++; }).After("D");

    auto& nodeG = graph.Node("G", [&]() { g = counter++; }).After("C").After("F").After("E");

    graph.Process();

    REQUIRE(graph.GetBuckets().size() == 5);

    REQUIRE(graph.GetBuckets()[0].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[1].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[2].m_nodes.size() == 3);
    REQUIRE(graph.GetBuckets()[3].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[4].m_nodes.size() == 1);

    REQUIRE(graph.GetBuckets()[0].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeA; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[1].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeB; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[2].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeC; }) !=
            Array<ExecutionNode*>::npos);
    REQUIRE(graph.GetBuckets()[2].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeD; }) !=
            Array<ExecutionNode*>::npos);
    REQUIRE(graph.GetBuckets()[2].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeE; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[3].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeF; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[4].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeG; }) !=
            Array<ExecutionNode*>::npos);

    graph.Execute();

    REQUIRE(a == 0);
    REQUIRE(b == 1);

    const bool cOk = (c == 2 || c == 3 || c == 4);
    REQUIRE(cOk);

    const bool dOk = (d == 2 || d == 3 || d == 4);
    REQUIRE(dOk);

    const bool eOk = (e == 2 || e == 3 || e == 4);
    REQUIRE(eOk);

    REQUIRE(f == 5);
    REQUIRE(g == 6);

    engine->Destroy();
}

std::atomic<int> s_counter = 0;

#define NewTestSystem(Name, Query)     \
    struct Name : public System<Query> \
    {                                  \
        virtual void Update() override \
        {                              \
            data = s_counter;          \
            s_counter++;               \
        }                              \
                                       \
        int data = -1;                 \
    };

using TestQueryA = Query<Reading<Transform2D>>;
using TestQueryB = Query<Reading<Transform2D>>;

using TestQueryC = Query<Reading<Transform2D>>;
using TestQueryD = Query<Writing<Transform2D>>;
using TestQueryE = Query<Reading<Transform2D>>;
using TestQueryF = Query<Writing<Transform2D>>;

using TestQueryG = Query<Writing<Transform2D>>;

NewTestSystem(TestA, TestQueryA);
NewTestSystem(TestB, TestQueryB);
NewTestSystem(TestC, TestQueryC);
NewTestSystem(TestD, TestQueryD);
NewTestSystem(TestE, TestQueryE);
NewTestSystem(TestF, TestQueryF);
NewTestSystem(TestG, TestQueryG);

TEST_CASE("Parallel system graph", "[Thread]")
{
    auto engine = CreateEngineFrom<EngineTest>(0, nullptr);

    World world;
    world.AddSystem<TestA>();
    world.AddSystem<TestB>();
    world.AddSystem<TestC>();
    world.AddSystem<TestD>();
    world.AddSystem<TestE>();
    world.AddSystem<TestF>();
    world.AddSystem<TestG>();

    SystemExecutionGraph graph(&world);

    auto& nodeA = graph.AddSystem<TestA>();

    auto& nodeB = graph.AddSystem<TestB>().After<TestA>();

    auto& nodeC = graph.AddSystem<TestC>().After<TestB>();
    auto& nodeD = graph.AddSystem<TestD>().After<TestB>();
    auto& nodeE = graph.AddSystem<TestE>().After<TestB>();

    auto& nodeF = graph.AddSystem<TestF>().After<TestD>();

    auto& nodeG = graph.AddSystem<TestG>().After<TestC>().After<TestF>().After<TestE>();

    graph.Process();

    REQUIRE(graph.GetBuckets().size() == 6);

    REQUIRE(graph.GetBuckets()[0].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[1].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[2].m_nodes.size() == 2);
    REQUIRE(graph.GetBuckets()[3].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[4].m_nodes.size() == 1);
    REQUIRE(graph.GetBuckets()[5].m_nodes.size() == 1);

    REQUIRE(graph.GetBuckets()[0].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeA; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[1].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeB; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[2].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeC; }) !=
            Array<ExecutionNode*>::npos);
    REQUIRE(graph.GetBuckets()[2].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeE; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[3].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeD; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[4].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeF; }) !=
            Array<ExecutionNode*>::npos);

    REQUIRE(graph.GetBuckets()[5].m_nodes.FindIf([&](ExecutionNode* node) { return node == &nodeG; }) !=
            Array<ExecutionNode*>::npos);

    graph.Execute();

    REQUIRE(((TestA*)nodeA.m_system)->data == 0);
    REQUIRE(((TestB*)nodeB.m_system)->data == 1);

    const bool cOk = (((TestC*)nodeC.m_system)->data == 2 || ((TestC*)nodeC.m_system)->data == 3);
    REQUIRE(cOk);

    const bool eOk = (((TestE*)nodeE.m_system)->data == 2 || ((TestE*)nodeE.m_system)->data == 3);
    REQUIRE(eOk);

    const bool dOk = ((TestD*)nodeD.m_system)->data == 4;
    REQUIRE(dOk);

    REQUIRE(((TestF*)nodeF.m_system)->data == 5);
    REQUIRE(((TestG*)nodeG.m_system)->data == 6);

    engine->Destroy();
}