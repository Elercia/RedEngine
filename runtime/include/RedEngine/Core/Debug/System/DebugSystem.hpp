#pragma once

#include "RedEngine/Entity/System.hpp"

namespace red
{
class DebugComponent;
class EventsComponent;

class DebugSystem : public System/*<SinglQuery<QueryRW<DebugComponent>>, 
                                    SinglQuery<QueryRO<EventsComponent>>,
                                  QueryGroup<QueryRO<Transform>>, SinglQuery<QueryRW<RendererComponent>>>*/
{
public:
    virtual void Init() override;
    virtual void Finalize() override;

    void Update() override;

private:
    static void RenderConsole(DebugComponent* debug);
    static void RenderEntityTree(DebugComponent* debug);
    static void RenderDebugPhysicsControls(DebugComponent* debug);
};
}  // namespace red
