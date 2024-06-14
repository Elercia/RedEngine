#pragma once

#include "RedEngine/Core/Event/Signal.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Input/InputDefinition.hpp"
#include "RedEngine/Math/Vector.hpp"

struct SDL_Window;

namespace red
{
class EventsComponent
{
    friend class EventSystem;

public:
    EventsComponent(Entity* entity);
    ~EventsComponent();

    [[nodiscard]] bool GetKey(KeyCodes::Enum key) const;
    [[nodiscard]] bool GetKeyUp(KeyCodes::Enum key) const;
    [[nodiscard]] bool GetKeyDown(KeyCodes::Enum key) const;
    [[nodiscard]] KeyState GetKeyState(KeyCodes::Enum key) const;

    [[nodiscard]] const Vector2i& GetMousePosition() const;
    [[nodiscard]] bool IsWindowResized(SDL_Window* window) const;

    [[nodiscard]] bool QuitRequested() const;

    void SendKeyEvent(KeyCodes::Enum key, KeyEventType::Enum type);

private:
    bool m_quitRequested{false};
    Vector2i m_mousePosition{0, 0};
    Array<uint32> m_windowIdResized{};

    std::array<KeyState, KeyCodes::MAX> m_keyStates;
};
RED_DECLARE_SINGLETON_COMPONENT(EventsComponent);

}  // namespace red