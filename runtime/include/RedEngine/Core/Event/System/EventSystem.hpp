#pragma once

#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Entity/System.hpp"

#include <array>

namespace red
{
class EventSystem : public System /*<SinglQuery<QueryRW<EventsComponent>>>*/
{
public:
    virtual ~EventSystem() = default;

    virtual void Init() override;
    virtual void Update() override;
};
}  // namespace red
