#include "RedEngine/Input/Component/UserInput.hpp"

#include "RedEngine/Input/InputModule.hpp"

#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Math/Hash.hpp"

namespace red
{
UserInputComponent::UserInputComponent()
{
}

UserInputComponent::~UserInputComponent() = default;

[[nodiscard]] bool UserInputComponent::GetKeyDown(const ActionKey& keyId) const
{
    auto it = m_state.find(keyId);
    if (it == m_state.end())
        return false;
    return it->second.isDown;
}

[[nodiscard]] bool UserInputComponent::GetKeyUp(const ActionKey& keyId) const
{
    auto it = m_state.find(keyId);
    if (it == m_state.end())
        return false;

    return it->second.isUp;
}

[[nodiscard]] bool UserInputComponent::GetKey(const ActionKey& keyId) const
{
    auto it = m_state.find(keyId);
    if (it == m_state.end())
        return false;

    return it->second.isPressed;
}

Vector2 UserInputComponent::GetAxis(const AxisKey& /*axisId*/) const
{
    return Vector2();
    // TODO manage axis
}

UserActionMapping UserInputComponent::GetActionMapping()
{
    return m_actionMapping;
}

void UserInputComponent::SetActionMapping(UserActionMapping mapping)
{
    m_actionMapping = std::move(mapping);
}
}  // namespace red