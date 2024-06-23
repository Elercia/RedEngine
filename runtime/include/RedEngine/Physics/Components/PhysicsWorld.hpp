#pragma once

#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Physics/Components/PhysicBody.hpp"

#include "box2d/b2_world_callbacks.h"

class b2World;

namespace red
{
class PhysicsDebugDrawer;
struct ContactListener;

float ConvertToPhysicsDistance(float f);
float ConvertFromPhysicsDistance(float f);
b2Vec2 ConvertToPhysicsVector(const Vector2& vector2);
Vector2 ConvertFromPhysicsVector(const b2Vec2& vector2);

class PhysicsWorld 
{
public:
    PhysicsWorld();
    ~PhysicsWorld();

    void InitPhysicsBody(PhysicBody* physicBody, const PhysicBodyCreationDesc& creationDesc);
    void FinalizePhysicsBody(PhysicBody* physicBody);

    void Step(float timeStep, int32 velocityIterations, int32 positionIterations);

    void ClearForces();

    void SetGravity(const Vector2& gravity);
    Vector2 GetGravity() const;

    const Array<CollisionInfo>& GetCollisions() const;
    const Array<TriggerInfo>& GetTriggers() const;

    void SetDebugDrawer(PhysicsDebugDrawer* drawer);
    void DrawDebug();

    void ClearContactInfo();
    void AddTriggerContact(PhysicBody* physicBody1, PhysicBody* physicBody2, Collider* collider1, Collider* collider2);
    void AddCollisionContact(PhysicBody* physicBody1, PhysicBody* physicBody2, Collider* collider1, Collider* collider2,
                             const b2Contact* contact);

private:
    b2World* m_internalPhysicsWorld;
    ContactListener* m_contactListener;

    Array<CollisionInfo> m_frameCollisionInfo;
    Array<TriggerInfo> m_frameTriggerInfo;
};

RED_DECLARE_SINGLETON_COMPONENT(PhysicsWorld);
}  // namespace red