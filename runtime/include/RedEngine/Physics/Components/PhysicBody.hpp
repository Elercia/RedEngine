#pragma once

#include "RedEngine//Math/Vector.hpp"
#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Event/Signal.hpp"
#include "RedEngine/Physics/ContactInfo.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

namespace red
{
class PhysicsWorld;

enum class ColiderType
{
    Circle,
    Edge,
    Polygon
};

struct ColliderDesc
{
    ColiderType colliderType;

    bool isTrigger{false};
    float friction{0.f};
    float restitution{1.f};

    struct
    {
        Vector2 center{0.f, 0.f};
        float radius{0.1f};
    } circle;

    struct
    {
        Vector2 start{0.f, 0.f};
        Vector2 end{0.f, 0.f};
    } edge;

    struct
    {
        Array<Vector2> points{};
    } polygon;
};

enum class PhysicsBodyType
{
    STATIC_BODY,
    DYNAMIC_BODY,
    KINEMATIC_BODY
};

struct PhysicBodyCreationDesc
{
    PhysicsBodyType type{PhysicsBodyType::STATIC_BODY};
    float linearDamping{0.f};
    float angularDamping{0.f};
    float gravityScale{1.f};

    Array<ColliderDesc> colliderDescs;
};

using ColliderId = uint32;

struct Collider
{
    b2Fixture* m_fixture{nullptr};

    bool IsTrigger() const
    {
        return m_fixture->IsSensor();
    }
};

// TODO Collision layers
class PhysicBody
{
    friend class PhysicSystem;
    friend class PhysicsWorld;

public:
    using OnCollisionSignalType = Signal<const CollisionInfo&>;
    using OnTriggerSignalType = Signal<const TriggerInfo&>;

    PhysicBody();
    ~PhysicBody();

    bool IsStatic() const;

    void ApplyForce(const Vector2& force, const Vector2& relativePosition);

    Map<ColliderId, Collider>& GetColliders();
    const Map<ColliderId, Collider>& GetColliders() const;

    b2Body* GetBody();
    const b2Body* GetBody() const;
    void SetBody(b2Body* body);

    OnTriggerSignalType m_triggerSignal;
    OnCollisionSignalType m_collisionSignal;

private:
    PhysicBodyCreationDesc m_desc;
    b2Body* m_body;
    Map<ColliderId, Collider> m_colliders;
    ColliderId m_nextColliderIndex;

    Array<OnCollisionSignalType::Slot> m_collisionSlots;
    Array<OnTriggerSignalType::Slot> m_triggerSlots;
};
}  // namespace red