#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/World.hpp"

#include <catch2/catch.hpp>

#include "TestUtils/TestUtils.hpp"

using namespace red;

struct WriteComp1
{
};

struct WriteComp2
{
};

struct ReadComp
{
};

struct NoNoNo
{
};

struct YesYesYes
{
};

struct SingletonTestComp
{
    int value = 0;
};

RED_DECLARE_SINGLETON_COMPONENT(SingletonTestComp)

TEST_CASE("Create query", "[ECS]")
{
    using TestQuery = Query<Writing<WriteComp1>, Writing<WriteComp2>, Reading<ReadComp>, Excluding<NoNoNo>,
                            Including<YesYesYes>, Writing<SingletonTestComp>>;
    static_assert(TestQuery::GetReadingComponentCount() == 1);
    static_assert(TestQuery::GetWritingComponentCount() == 3);
    static_assert(TestQuery::GetIncludingComponentCount() == 5);
    static_assert(TestQuery::GetExcludingComponentCount() == 1);

    static_assert(TestQuery::GetExcludingComponents().size() == 1);

    static_assert(std::find(TestQuery::GetReadingComponents().begin(), TestQuery::GetReadingComponents().end(),
                            TypeInfo<ReadComp>()) != TestQuery::GetReadingComponents().end());

    static_assert(std::find(TestQuery::GetWritingComponents().begin(), TestQuery::GetWritingComponents().end(),
                            TypeInfo<WriteComp1>()) != TestQuery::GetWritingComponents().end());
    static_assert(std::find(TestQuery::GetWritingComponents().begin(), TestQuery::GetWritingComponents().end(),
                            TypeInfo<WriteComp2>()) != TestQuery::GetWritingComponents().end());
    static_assert(std::find(TestQuery::GetWritingComponents().begin(), TestQuery::GetWritingComponents().end(),
                            TypeInfo<SingletonTestComp>()) != TestQuery::GetWritingComponents().end());

    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<WriteComp1>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<WriteComp2>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<ReadComp>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<YesYesYes>()) != TestQuery::GetIncludingComponents().end());
    static_assert(std::find(TestQuery::GetIncludingComponents().begin(), TestQuery::GetIncludingComponents().end(),
                            TypeInfo<SingletonTestComp>()) != TestQuery::GetIncludingComponents().end());

    static_assert(std::find(TestQuery::GetExcludingComponents().begin(), TestQuery::GetExcludingComponents().end(),
                            TypeInfo<NoNoNo>()) != TestQuery::GetExcludingComponents().end());

    TestQuery testQuery;
    static_assert(TupleSize<std::decay<decltype(testQuery.GetEntitiesComponents()[0])>::type>::value ==
                  4);  // 3 + EntityId
    static_assert(TupleSize<decltype(testQuery.GetSingletonComponents())>::value == 1);

    using NoReadQuery = Query<Writing<WriteComp1>, Writing<WriteComp2>>;

    static_assert(NoReadQuery::GetReadingComponentCount() == 0);
    static_assert(NoReadQuery::GetWritingComponentCount() == 2);
    static_assert(NoReadQuery::GetIncludingComponentCount() == 2);
    static_assert(NoReadQuery::GetExcludingComponentCount() == 0);
}

int g_updateCount = 0;

using TestSystemTestQuery = Query<Writing<WriteComp1>, Reading<SingletonTestComp>>;
struct TestSystem : public red::System<TestSystemTestQuery>
{
    virtual void Update() override
    {
        auto singls = m_query.GetSingletonComponents();
        auto* singlComp = std::get<const SingletonTestComp*>(singls);

        REQUIRE(singlComp != NULL);
        REQUIRE(singlComp->value == 1);

        g_updateCount++;
    }
};

struct TestInitializer : public SystemInitializer
{
    virtual void Init() override
    {
        auto comp = m_world->GetSingletonComponent<SingletonTestComp>();

        comp->value = 1;
    }

    virtual void Finalize() override
    {
    }
};

TEST_CASE("System add/remove", "[ECS]")
{
    World world;

    world.RegisterComponent<SingletonTestComp>();

    world.AddSystemInitializer<TestInitializer>();
    world.AddSystem<TestSystem>();

    world.Init();

    world.Update();

    world.Finalize();

    world.RemoveSystem<TestSystem>();
    world.RemoveSystemInitializer<TestInitializer>();


    REQUIRE(g_updateCount == 1);
}