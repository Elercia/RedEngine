#pragma once

#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Entity/System.hpp"

#include <array>

namespace red
{
using EventSystemQuery = Query<Writing<EventsComponent>>;
class EventSystem : public System<EventSystemQuery>
{
public:
    virtual void Update() override;
};
}  // namespace red
