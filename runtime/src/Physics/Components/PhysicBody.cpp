#include "RedEngine/Physics/Components/PhysicBody.hpp"

#include "RedEngine/Physics/PhysicsModule.hpp"

#include "RedEngine/Physics/Components/PhysicsWorld.hpp"

#include <box2d/b2_circle_shape.h>
#include <box2d/b2_edge_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_shape.h>

namespace red
{

PhysicBody::PhysicBody() : m_desc(), m_body(nullptr), m_nextColliderIndex(0)
{
}

PhysicBody::~PhysicBody()
{
    RedAssert(m_body == nullptr, "PhysicBody has not been destroyed");
}

bool PhysicBody::IsStatic() const
{
    return m_desc.type == PhysicsBodyType::STATIC_BODY;
}

void PhysicBody::ApplyForce(const Vector2& force, const Vector2& relativePosition)
{
    auto worldPosition = ConvertFromPhysicsVector(m_body->GetPosition()) + relativePosition;

    m_body->ApplyForce(ConvertToPhysicsVector(force), ConvertToPhysicsVector(worldPosition), true);
}

b2Body* PhysicBody::GetBody()
{
    return m_body;
}

const b2Body* PhysicBody::GetBody() const
{
    return m_body;
}

void PhysicBody::SetBody(b2Body* body)
{
    m_body = body;
}

Map<ColliderId, Collider>& PhysicBody::GetColliders()
{
    return m_colliders;
}

const Map<ColliderId, Collider>& PhysicBody::GetColliders() const
{
    return m_colliders;
}

}  // namespace red
