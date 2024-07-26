#pragma once

#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

#include <functional>

namespace red
{
using ComponentVoidPtr = void*;

template <typename COMP>
struct ComponentTraits
{
    static constexpr bool IsSingleton = false;
};

#define RED_DECLARE_SINGLETON_COMPONENT(COMP)     \
    template <>                                   \
    struct ComponentTraits<COMP>                  \
    {                                             \
        static constexpr bool IsSingleton = true; \
    };

struct ComponentMetadata
{
    TypeTraits traits;
    bool isSingleton;
    uint32 size;
    std::function<void(ComponentVoidPtr)> construct;
    std::function<void(ComponentVoidPtr)> destruct;
};

class ComponentRegistry
{
public:
    template <typename COMP>
    bool RegisterComponent();

    bool RegisterComponent(TypeTraits traits, bool isSingleton, uint32 size,
                           std::function<void(ComponentVoidPtr)> construct,
                           std::function<void(ComponentVoidPtr)> destruct);

    template <typename COMP>
    const ComponentMetadata* GetRegisteredComponentMetadata() const;
    const ComponentMetadata* GetRegisteredComponentMetadata(StringView componentName) const;
    const ComponentMetadata* GetRegisteredComponentMetadata(TypeTraitsId typeId) const;

    const Map<TypeTraits, ComponentMetadata>& GetComponentsMetadata() const;

private:
    Map<TypeTraits, ComponentMetadata> m_registeredComponents;
};
}  // namespace red

#include "inl/ComponentRegistry.inl"