#include "RedEngine/Input/System/UserInputSystem.hpp"

#include "RedEngine/Input/InputModule.hpp"

#include "RedEngine/Core/Configuration/IniReader.hpp"
#include "RedEngine/Core/Configuration/UserInputHelper.hpp"
#include "RedEngine/Core/Debug/Profiler.hpp"
#include "RedEngine/Core/Engine.hpp"
#include "RedEngine/Core/Event/Component/EventsComponent.hpp"

#include <SDL2/SDL.h>

namespace red
{
void UserInputSystem::Update()
{
    PROFILER_EVENT_CATEGORY("UserInputSystem::Update", ProfilerCategory::Input);

    const auto* eventsSystem = std::get<const EventsComponent*>(m_query.GetSingletonComponents());
    auto* inputComponent = std::get<UserInputComponent*>(m_query.GetSingletonComponents());
    
    for (auto& actionMapping : inputComponent->m_actionMapping)
    {
        const auto& actionName = actionMapping.first;
        const auto& mapping = actionMapping.second;

        KeyState oldState = inputComponent->m_state[actionName];
        KeyState mappingState = eventsSystem->GetKeyState(mapping.mapping);

        Array<KeyState> states;

        for (size_t i = 0; i < g_modifierKeys.size(); i++)
        {
            if (mapping.modifiers.test(i))
            {
                states.push_back(eventsSystem->GetKeyState(g_modifierKeys[i].keyCode));
            }
        }

        // Push the mapping
        states.push_back(mappingState);

        auto resultState = AglomerateKeyStates(oldState, states);

        inputComponent->m_state[actionName] = resultState;

        if (resultState.isDown)
        {
            RED_LOG_TRACE("User action {} is down", actionName);
        }

        if (resultState.isUp)
        {
            RED_LOG_TRACE("User action {} is up", actionName);
        }
    }
}

KeyState UserInputSystem::AglomerateKeyStates(const KeyState& oldState, const Array<KeyState>& states)
{
    KeyState resultState = {false, false, false};
    resultState.isPressed = states[0].isPressed;

    for (const auto& state : states)
    {
        // A user input is pressed only if all the required keys are pressed
        resultState.isPressed = state.isPressed && resultState.isPressed;
    }

    if (!oldState.isPressed && resultState.isPressed)
        resultState.isDown = true;

    if (oldState.isPressed && !resultState.isPressed)
        resultState.isUp = true;

    return resultState;
}

}  // namespace red
