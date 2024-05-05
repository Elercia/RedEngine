#include "RedEngine/Entity/ComponentRegistry.hpp"

namespace red
{
bool ComponentRegistry::RegisterComponent(TypeTraits traits, bool isSingleton, uint32 size,
    std::function<void(ComponentVoidPtr)> construct,std::function<void(ComponentVoidPtr)> destruct)
{
    if (m_registeredComponents.find(traits) != m_registeredComponents.end())
        return false;

    ComponentMetadata metadata;
    metadata.traits = traits;
    metadata.isSingleton = isSingleton;
    metadata.size = size;
    metadata.construct = construct;
    metadata.destruct = destruct;

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

const Map<TypeTraits, ComponentMetadata>& red::ComponentRegistry::GetComponentsMetadata() const
{
    return m_registeredComponents;
}

}  // namespace red