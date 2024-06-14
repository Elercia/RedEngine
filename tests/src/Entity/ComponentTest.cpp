#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/ComponentManager.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Math/Vector.hpp"

#include <catch2/catch.hpp>

#include "TestUtils/TestUtils.hpp"

using namespace red;

struct TestComponent
{
    TestComponent()
    {
        a.x = 10.f;
        a.y = 100.f;
    }
    Vector2 a;
};

struct TestComponent2
{
    TestComponent2()
    {
        a.x = 10.f;
        a.y = 100.f;
    }
    Vector2 a;
};

struct UnknownComponent
{
    int a;
};

struct ConstructDestructComp
{
    static int nbConstruction;
    static int nbDestruction;
    static void ResetCounters()
    {
        nbConstruction = 0;
        nbDestruction = 0;
    }

    ConstructDestructComp()
    {
        nbConstruction++;
    }

    ~ConstructDestructComp()
    {
        nbDestruction++;
    }
};

int ConstructDestructComp::nbConstruction = 0;
int ConstructDestructComp::nbDestruction = 0;

struct ConstructDestructCompSingleton
{
    static int nbConstruction;
    static int nbDestruction;
    static void ResetCounters()
    {
        nbConstruction = 0;
        nbDestruction = 0;
    }

    ConstructDestructCompSingleton()
    {
        nbConstruction++;
    }

    ~ConstructDestructCompSingleton()
    {
        nbDestruction++;
    }
};

int ConstructDestructCompSingleton::nbConstruction = 0;
int ConstructDestructCompSingleton::nbDestruction = 0;

struct SingletonComp
{
    SingletonComp()
    {
        a.x = 10.f;
        a.y = 100.f;
    }
    Vector2 a;
};

RED_DECLARE_SINGLETON_COMPONENT(ConstructDestructCompSingleton)
RED_DECLARE_SINGLETON_COMPONENT(SingletonComp)

TEST_CASE("Register components", "[ECS]")
{
    ComponentRegistry registry;

    REQUIRE(registry.RegisterComponent<TestComponent>());

    SECTION("Get metadata from template")
    {
        auto* metadata = registry.GetRegisteredComponentMetadata<TestComponent>();
        REQUIRE(metadata != nullptr);
        REQUIRE(metadata->traits.typeId == TypeInfo<TestComponent>().typeId);
    }

    SECTION("Get metadata from name")
    {
        auto* metadata = registry.GetRegisteredComponentMetadata("TestComponent");
        REQUIRE(metadata != nullptr);
        REQUIRE(metadata->traits.typeId == TypeInfo<TestComponent>().typeId);
        REQUIRE(metadata->isSingleton == false);
    }

    SECTION("Get metadata from typeid")
    {
        auto* metadata = registry.GetRegisteredComponentMetadata(TypeInfo<TestComponent>().typeId);
        REQUIRE(metadata != nullptr);
        REQUIRE(metadata->traits.typeId == TypeInfo<TestComponent>().typeId);
        REQUIRE(metadata->isSingleton == false);
    }
}

TEST_CASE("Create component", "[ECS]")
{
    ComponentRegistry registry;
    ComponentManager manager(&registry);

    EntityId entity = 1;

    REQUIRE(registry.RegisterComponent<TestComponent>());

    manager.Init();

    SECTION("Create unknown component")
    {
        auto* comp = manager.CreateComponent<UnknownComponent>(entity);

        REQUIRE(comp == nullptr);
    }

    SECTION("Create component from type")
    {
        auto* comp = manager.CreateComponent<TestComponent>(entity);

        REQUIRE(comp != nullptr);
        REQUIRE(comp->a.x == 10.f);
        REQUIRE(comp->a.y == 100.f);
    }

    SECTION("Create component from traits")
    {
        auto* comp = (TestComponent*)manager.CreateComponent(TypeInfo<TestComponent>(), entity);

        REQUIRE(comp != nullptr);
        REQUIRE(comp->a.x == 10.f);
        REQUIRE(comp->a.y == 100.f);
    }

    manager.Finalize();
}

TEST_CASE("Get components", "[ECS]")
{
    ComponentRegistry registry;
    ComponentManager manager(&registry);

    REQUIRE(registry.RegisterComponent<TestComponent>());
    REQUIRE(registry.RegisterComponent<TestComponent2>());

    EntityId entity1 = 1;
    EntityId entity2 = 2;
    EntityId entity3 = 3;

    manager.Init();

    {
        auto* comp1 = manager.CreateComponent<TestComponent>(entity1);
        auto* comp2 = manager.CreateComponent<TestComponent>(entity2);
        REQUIRE(comp1 != nullptr);
        REQUIRE(comp2 != nullptr);

        auto* comp3 = manager.CreateComponent<TestComponent2>(entity1);
        auto* comp3_2 = manager.CreateComponent<TestComponent2>(entity1);
        REQUIRE(comp3 != nullptr);
        REQUIRE(comp3 == comp3_2);

        auto* comp4 = manager.CreateComponent<TestComponent2>(entity3);
        REQUIRE(comp4 != nullptr);

        SECTION("Get one component type")
        {
            auto comps = manager.GetComponents<TestComponent>();
            REQUIRE(comps.size() == 2);

            REQUIRE(std::get<0>(comps[0]) == entity1);
            REQUIRE(std::get<1>(comps[0]) == comp1);

            REQUIRE(std::get<0>(comps[1]) == entity2);
            REQUIRE(std::get<1>(comps[1]) == comp2);
        }

        SECTION("Get both components types")
        {
            auto comps = manager.GetComponents<TestComponent, TestComponent2>();
            REQUIRE(comps.size() == 1);

            REQUIRE(std::get<0>(comps[0]) == entity1);
            REQUIRE(std::get<1>(comps[0]) == comp1);
            REQUIRE(std::get<2>(comps[0]) == comp3);
        }
    }

    manager.Finalize();
}

TEST_CASE("Singleton components", "[ECS]")
{
    ComponentRegistry registry;
    ComponentManager manager(&registry);

    REQUIRE(registry.RegisterComponent<SingletonComp>());

    manager.Init();

    auto comp = manager.GetSingletonComponent<SingletonComp>();
    REQUIRE(comp != nullptr);
    REQUIRE(comp->a.x == 10.f);
    REQUIRE(comp->a.y == 100.f);

    manager.Finalize();
}

TEST_CASE("Singleton components construct destruct", "[ECS]")
{
    ConstructDestructCompSingleton::ResetCounters();

    ComponentRegistry registry;
    ComponentManager manager(&registry);

    REQUIRE(registry.RegisterComponent<ConstructDestructCompSingleton>());

    manager.Init();

    REQUIRE(ConstructDestructCompSingleton::nbConstruction == 1);

    auto comp = manager.GetSingletonComponent<ConstructDestructCompSingleton>();
    REQUIRE(comp != nullptr);
    REQUIRE(ConstructDestructCompSingleton::nbConstruction == 1);
    REQUIRE(ConstructDestructCompSingleton::nbDestruction == 0);

    manager.Finalize();

    REQUIRE(ConstructDestructCompSingleton::nbConstruction == 1);
    REQUIRE(ConstructDestructCompSingleton::nbDestruction == 1);
}

TEST_CASE("Components construct destruct", "[ECS]")
{
    ConstructDestructComp::ResetCounters();

    ComponentRegistry registry;
    ComponentManager manager(&registry);

    REQUIRE(registry.RegisterComponent<ConstructDestructComp>());

    manager.Init();
    REQUIRE(ConstructDestructComp::nbConstruction == 0);

    EntityId entity1 = 1;
    EntityId entity2 = 2;
    EntityId entity3 = 3;

    manager.CreateComponent<ConstructDestructComp>(entity1);
    manager.CreateComponent<ConstructDestructComp>(entity2);
    manager.CreateComponent<ConstructDestructComp>(entity3);

    REQUIRE(ConstructDestructComp::nbConstruction == 3);

    manager.RemoveComponent<ConstructDestructComp>(entity1);
    manager.RemoveComponent<ConstructDestructComp>(entity2);

    REQUIRE(ConstructDestructComp::nbConstruction == 3);
    REQUIRE(ConstructDestructComp::nbDestruction == 2);

    manager.Finalize();

    REQUIRE(ConstructDestructComp::nbDestruction == 3);
}