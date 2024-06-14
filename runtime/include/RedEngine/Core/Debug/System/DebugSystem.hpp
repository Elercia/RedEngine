#pragma once

#include "RedEngine/Core/Debug/Component/DebugComponent.hpp"
#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/Transform.hpp"

namespace red
{
class DebugSystemInitializer : public SystemInitializer
{
    void Init() override;
    void Finalize() override;
};

using DebugSystemQuery = Query<Writing<DebugComponent>, Reading<EventsComponent>, Reading<Transform2D>>;
class DebugSystem : public System<DebugSystemQuery>
{
public:
    void Update() override;

    static void RenderConsole(DebugComponent* debug);
    static void RenderEntityTree(DebugComponent* debug);
    static void RenderDebugPhysicsControls(DebugComponent* debug);
};
}  // namespace red
