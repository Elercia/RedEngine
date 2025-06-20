#include "RedEngine/Core/Memory/MemoryProfiler.hpp"

#include <algorithm>
#include <catch2/catch.hpp>
#include <random>

using namespace red;

TEST_CASE("Simple memory profiling", "[MEMORY]")
{
    MemoryProfiler::ResetUsage();

    void* a = red_malloc(sizeof(int));

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 1);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == sizeof(int));

    red_free(a);

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 0);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == 0);

    REQUIRE(MemoryProfiler::GetUsage().peekAllocationCount == 1);
    REQUIRE(MemoryProfiler::GetUsage().peekAllocated == sizeof(int));
}

TEST_CASE("Memory profiling x100", "[MEMORY]")
{
    MemoryProfiler::ResetUsage();

    std::vector<void*> allocs;

    for (int i = 0; i < 100; i++)
    {
        void* a1 = red_malloc(sizeof(int));
        allocs.push_back(a1);

        void* a2 = red_malloc(sizeof(float));
        allocs.push_back(a2);
    }

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 200);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == ((100 * sizeof(int)) + (100 * sizeof(float))));

    REQUIRE(MemoryProfiler::GetUsage().peekAllocationCount == 200);
    REQUIRE(MemoryProfiler::GetUsage().peekAllocated == ((100 * sizeof(int)) + (100 * sizeof(float))));

    std::random_device rd;
    std::mt19937 g(rd());
    g.seed(MemoryGuard);

    std::shuffle(allocs.begin(), allocs.end(), g);

    for (void* a : allocs)
    {
        red_free(a);
    }

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 0);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == 0);

    REQUIRE(MemoryProfiler::GetUsage().peekAllocationCount == 200);
    REQUIRE(MemoryProfiler::GetUsage().peekAllocated == ((100 * sizeof(int)) + (100 * sizeof(float))));
}

struct MemoryTest
{
    static bool ctrCalled;
    static bool dctrCalled;

    int a;
    float f;

    MemoryTest()
    {
        ctrCalled = true;
        a = 0;
        f = 0.f;
    }

    ~MemoryTest()
    {
        dctrCalled = true;
    }
};

bool MemoryTest::ctrCalled = false;
bool MemoryTest::dctrCalled = false;

TEST_CASE("New / delete", "[MEMORY]")
{
    MemoryProfiler::ResetUsage();
    MemoryTest::ctrCalled = false;
    MemoryTest::dctrCalled = false;

    MemoryTest* a = red_new(MemoryTest);

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 1);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == sizeof(MemoryTest));

    red_delete(a);

    REQUIRE(MemoryTest::ctrCalled);
    REQUIRE(MemoryTest::dctrCalled);

    REQUIRE(MemoryProfiler::GetUsage().currentAllocationCount == 0);
    REQUIRE(MemoryProfiler::GetUsage().currentlyAllocated == 0);

    REQUIRE(MemoryProfiler::GetUsage().peekAllocationCount == 1);
    REQUIRE(MemoryProfiler::GetUsage().peekAllocated == sizeof(MemoryTest));
}