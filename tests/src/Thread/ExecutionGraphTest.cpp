#include "RedEngine/Thread/ExecutionGraph.hpp"

#include <catch2/catch.hpp>

using namespace red;

TEST_CASE("Simple execution graph", "[Thread]")
{
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
}
