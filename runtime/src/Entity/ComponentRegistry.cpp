#include "RedEngine/Entity/ComponentRegistry.hpp"

namespace red
{
bool ComponentRegistry::RegisterComponent(TypeTraits traits)
{
    if (m_registeredComponents.find(traits) != m_registeredComponents.end())
        return false;

    ComponentMetadata metadata;
    metadata.traits = traits;

    m_registeredComponents.insert({traits, metadata});

    return true;
}

const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata(StringView componentName) const
{
    auto it = m_registeredComponents.find(componentName);

    if (it != m_registeredComponents.end())
        return &it->second;

    RedAssertNotReached();

    return nullptr;
}

const ComponentMetadata* ComponentRegistry::GetRegisteredComponentMetadata(TypeTraitsId typeId) const
{
    auto it = m_registeredComponents.find(typeId);

    if (it != m_registeredComponents.end())
        return &it->second;

    return nullptr;
}

}  // namespace red