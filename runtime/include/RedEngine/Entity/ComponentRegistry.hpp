#pragma once

#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{
struct ComponentMetadata
{
    TypeTraits traits;
    bool isSingleton;
};

class ComponentRegistry
{
public:
    template <typename COMP>
    bool RegisterComponent(bool isSingleton);

    bool RegisterComponent(TypeTraits traits, bool isSingleton);

    template <typename COMP>
    const ComponentMetadata* GetRegisteredComponentMetadata() const;
    const ComponentMetadata* GetRegisteredComponentMetadata(StringView componentName) const;
    const ComponentMetadata* GetRegisteredComponentMetadata(TypeTraitsId typeId) const;

    Map<TypeTraits, ComponentMetadata> m_registeredComponents;
};
}  // namespace red

#include "inl/ComponentRegistry.inl"