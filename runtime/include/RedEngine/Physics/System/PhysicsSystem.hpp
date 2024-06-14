#pragma once

#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/Transform.hpp"
#include "RedEngine/Physics/Components/PhysicBody.hpp"
#include "RedEngine/Physics/Components/PhysicsWorld.hpp"

class b2Contact;

namespace red
{
using UpdatePhysicsFromEntitiesSystemQuery = Query<Reading<Transform2D>, Writing<PhysicBody>, Writing<PhysicsWorld>>;

class UpdatePhysicsFromEntitiesSystem : public System<UpdatePhysicsFromEntitiesSystemQuery>
{
    void Update() override;
};

using UpdateEntitiesFromPhysicsSystemQuery = Query<Writing<Transform2D>, Reading<PhysicBody>, Writing<PhysicsWorld>>;

class UpdateEntitiesFromPhysicsSystem : public System<UpdateEntitiesFromPhysicsSystemQuery>
{
    void Update() override;

private:
    void ManageCollisions();
    void ManageTriggers();
};

using UpdatePhysicSystemQuery = Query<Writing<PhysicsWorld>>;
class UpdatePhysicSystem : public System<UpdatePhysicSystemQuery>
{
public:
    void Update() override;

private:
    static const float timeStep;
    static const int32 velocityIterations;
    static const int32 positionIterations;
};
}  // namespace red