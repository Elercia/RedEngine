#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/ComponentManager.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Math/Vector.hpp"

#include <catch2/catch.hpp>

#include "TestUtils/TestUtils.hpp"

using namespace red;

struct TestComponent
{
    Vector2 a;
};

TEST_CASE("Register components", "[ECS]")
{
    ComponentRegistry registry;

    REQUIRE(registry.RegisterComponent<TestComponent>(false));

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

TEST_CASE("Component manager", "[ECS]")
{
    ComponentRegistry registry;
    ComponentManager manager(&registry);

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
    }

    SECTION("Get metadata from typeid")
    {
        auto* metadata = registry.GetRegisteredComponentMetadata(TypeInfo<TestComponent>().typeId);
        REQUIRE(metadata != nullptr);
        REQUIRE(metadata->traits.typeId == TypeInfo<TestComponent>().typeId);
    }
}