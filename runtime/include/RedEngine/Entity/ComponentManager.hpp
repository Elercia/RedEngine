#pragma once

#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Memory/PoolAllocator.hpp"
#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Utils/Tuple.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{
class ComponentRegistry;
struct ComponentMetadata;

using ComponentVoidPtr = void*;

struct ComponentPool
{
    VirtualPoolAllocator allocator;
    Map<EntityId, ComponentVoidPtr> components;
    const ComponentMetadata* registryMetadata;
};

struct SingletonComponent
{
    ComponentVoidPtr component;
    const ComponentMetadata* registryMetadata;
};

class ComponentManager
{
public:
    ComponentManager(ComponentRegistry* registry);

    // Will create the pool for the registered components (pre-allocate memory & create the singleton components)
    void Init();
    void Finalize();

    template <typename COMP>
    COMP* CreateComponent(EntityId entityId);
    ComponentVoidPtr CreateComponent(const TypeTraits& componentTraits, EntityId entityId);

    template <typename COMP>
    void RemoveComponent(EntityId entityId);
    void RemoveComponent(const TypeTraits& componentTraits, EntityId entityId);

    // Retreive a list of entities that matches the component list given as template parametters
    template <typename... COMPS>
    Array<Tuple<EntityId, COMPS*...>> GetComponents();

    template <typename SINGL>
    SINGL* GetSingletonComponent();

private:
    template <typename POOL>
    ComponentPool* GetPool();

    template <typename... COMPS>
    void Intersect(Array<Tuple<EntityId, COMPS*...>>& outResult, ArrayView<ComponentPool*> pools);

private:
    ComponentRegistry* m_registry;
    Map<TypeTraits, ComponentPool> m_pools;
    Map<TypeTraits, SingletonComponent> m_singletons;
};
}  // namespace red

#include "inl/ComponentManager.inl"