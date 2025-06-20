#pragma once

#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Input/InputDefinition.hpp"
#include "RedEngine/Input/UserInputDefinition.hpp"
#include "RedEngine/Math/Vector.hpp"

namespace red
{
// TODO handle joysticks and gamepads in general
class UserInputComponent
{
    friend class UserInputSystem;

public:
    explicit UserInputComponent();
    ~UserInputComponent();

    [[nodiscard]] bool GetKeyDown(const ActionKey& keyId) const;
    [[nodiscard]] bool GetKeyUp(const ActionKey& keyId) const;
    [[nodiscard]] bool GetKey(const ActionKey& keyId) const;

    [[nodiscard]] Vector2 GetAxis(const AxisKey& axisId) const;

    [[nodiscard]] UserActionMapping GetActionMapping();
    void SetActionMapping(UserActionMapping mapping);

private:
    UserActionMapping m_actionMapping{};
    UserActionState m_state{};
};

RED_DECLARE_SINGLETON_COMPONENT(UserInputComponent);

}  // namespace red
