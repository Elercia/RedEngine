#include "RedEngine/Physics/PhysicsModule.hpp"

#include "RedEngine/Core/Debug/Profiler.hpp"
#include "RedEngine/Core/Engine.hpp"
#include "RedEngine/Physics/Components/PhysicBody.hpp"
#include "RedEngine/Physics/ContactInfo.hpp"
#include "RedEngine/Physics/System/PhysicsSystem.hpp"

#include <box2d/b2_contact.h>

namespace red
{
void UpdatePhysicsFromEntitiesSystem::Update()
{
    PROFILER_EVENT_CATEGORY("UpdatePhysicsFromEntitiesSystem::Update", ProfilerCategory::Physics)

    auto* physicsWorld = std::get<PhysicsWorld*>(m_query.GetSingletonComponents());
    physicsWorld->ClearForces();

    auto bodies = m_query.GetEntitiesComponents();

    for (auto& tuple : bodies)
    {
        auto transform = std::get<const Transform2D*>(tuple);
        auto physicBody = std::get<PhysicBody*>(tuple);

        if (physicBody->IsStatic())
        {
            continue;
        }

        // FIXME : Manage object scale. Need to scale the different fixtures of the body
        physicBody->GetBody()->SetTransform(ConvertToPhysicsVector(transform->localPosition),
                                            transform->localRotationRad);
    }
}

const float UpdatePhysicSystem::timeStep = 1.0f / 60.f;
const int32 UpdatePhysicSystem::velocityIterations = 6;
const int32 UpdatePhysicSystem::positionIterations = 2;

void UpdatePhysicSystem::Update()
{
    PROFILER_EVENT_CATEGORY("UpdatePhysicSystem::Update", ProfilerCategory::Physics)

    auto* physicsWorld = std::get<PhysicsWorld*>(m_query.GetSingletonComponents());

    physicsWorld->Step(timeStep, velocityIterations, positionIterations);
}

void UpdateEntitiesFromPhysicsSystem::Update()
{
    PROFILER_EVENT_CATEGORY("UpdateEntitiesFromPhysicsSystem::Update", ProfilerCategory::Physics)

    auto& scheduler = Engine::GetInstance()->GetScheduler();

    auto bodies = m_query.GetEntitiesComponents();

    auto wgEnd = scheduler.SplitWorkLoad(
        bodies.size(),
        [&](const ThreadScheduler::WorkRange& range, int /*taskId*/)
        {
            PROFILER_EVENT_CATEGORY("PhysicSystem::CopyBackPositions", ProfilerCategory::Physics)

            for (int i = range.start; i < range.end; i++)
            {
                auto& tuple = bodies[i];
                auto transform = std::get<Transform2D*>(tuple);
                auto physicBody = std::get<const PhysicBody*>(tuple);

                if (physicBody->IsStatic())
                {
                    continue;
                }

                transform->localPosition = ConvertFromPhysicsVector(physicBody->GetBody()->GetPosition());
                transform->localRotationRad = physicBody->GetBody()->GetAngle();
            }
        });

    wgEnd.Wait();

    ManageCollisions();
    ManageTriggers();
}

void UpdateEntitiesFromPhysicsSystem::ManageCollisions()
{
    PROFILER_EVENT_CATEGORY("UpdateEntitiesFromPhysicsSystem::ManageCollisions", ProfilerCategory::Physics)

    auto* physicsWorld = std::get<PhysicsWorld*>(m_query.GetSingletonComponents());

    for (const auto& constCollision : physicsWorld->GetCollisions())
    {
        auto collision = constCollision;  // copy

        collision.firstPhysicBody->m_collisionSignal.emit(collision);

        collision.SwapFirstSecond();

        collision.firstPhysicBody->m_collisionSignal.emit(collision);
    }
}

void UpdateEntitiesFromPhysicsSystem::ManageTriggers()
{
    PROFILER_EVENT_CATEGORY("UpdateEntitiesFromPhysicsSystem::ManageTriggers", ProfilerCategory::Physics)

    auto* physicsWorld = std::get<PhysicsWorld*>(m_query.GetSingletonComponents());

    const auto& triggers = physicsWorld->GetTriggers();

    for (const auto& constTrigger : triggers)
    {
        auto triggerInfo = constTrigger;  // copy

        triggerInfo.firstPhysicBody->m_triggerSignal(triggerInfo);

        triggerInfo.SwapFirstSecond();

        triggerInfo.secondPhysicBody->m_triggerSignal(triggerInfo);
    }
}
}  // namespace red