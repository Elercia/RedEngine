#include "RedEngine/Physics/Components/PhysicBody.hpp"
#include "RedEngine/Physics/System/PhysicsSystem.hpp"

#include <catch2/catch.hpp>

#include "EngineTest.hpp"

int size(b2Fixture* fixture)
{
    int nb = 0;

    while (fixture != nullptr)
    {
        nb++;
        fixture = fixture->GetNext();
    }
    return nb;
}

TEST_CASE("Component binding", "[PHYSICS]")
{
    using namespace red;

    World world;
    world.RegisterComponent<PhysicBody>();
    world.RegisterComponent<PhysicsWorld>();

    world.Init();

    EntityId entity = world.CreateEntity();

    PhysicsWorld* physicWorld = world.GetSingletonComponent<PhysicsWorld>();

    red::PhysicBodyCreationDesc desc = {
        .type = red::PhysicsBodyType::DYNAMIC_BODY,
        .colliderDescs = {
            ColliderDesc{.colliderType = ColiderType::Circle, .isTrigger = false, .circle = {.center = {5.F, 5.F}, .radius = 5.F}},
            ColliderDesc{.colliderType = ColiderType::Polygon, .isTrigger = false, .polygon = {.points = {{0, 0}, {30, 0}, {30, 100}, {0, 100}}}},
            ColliderDesc{.colliderType = ColiderType::Edge, .isTrigger = false, .edge = {.start = {0.f, 0.f}, .end = {1.f, 0.f}}}
        }
    };

    auto* body = world.AddComponentToEntity<PhysicBody>(entity);

    physicWorld->InitPhysicsBody(body, desc);

    REQUIRE(size(body->GetBody()->GetFixtureList()) == 3);

    physicWorld->FinalizePhysicsBody(body);

    world.Finalize();
}
