#pragma once

#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Input/Component/UserInput.hpp"

namespace red
{
using UserInputSystemQuery = Query<Writing<UserInputComponent>, Reading<EventsComponent>>;
class UserInputSystem : public System<UserInputSystemQuery>
{
public:

    void Update() override;

private:
    KeyState AglomerateKeyStates(const KeyState& oldState, const Array<KeyState>& states);
};
}  // namespace red