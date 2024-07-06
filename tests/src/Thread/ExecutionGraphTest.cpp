#include "RedEngine/Core/Engine.hpp"
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

    REQUIRE(f == 5 );
    REQUIRE(g == 6 );


    engine->Destroy();
}