#include "RedEngine/Entity/ComponentManager.hpp"

#include "RedEngine/Entity/EntityModule.hpp"

#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"

namespace red
{
ComponentManager::ComponentManager(ComponentRegistry* registry) : m_registry(registry)
{
}

void ComponentManager::Init()
{
    const auto& registeredComponents = m_registry->GetComponentsMetadata();

    for (const auto& it : registeredComponents)
    {
        if (it->isSingleton)
        {
            SingletonComponent comp;
            comp.component = (ComponentVoidPtr)red_malloc(it->size);
            comp.registryMetadata = &(it.second);

            it->construct(comp.component);

            m_singletons.insert({it->traits, comp});
        }
        else
        {
            ComponentPool pool{.allocator = VirtualPoolAllocator(it->size, 100), .registryMetadata = &(it.second)};

            m_pools.insert({it->traits, std::move(pool)});
        }
    }
}

void ComponentManager::Finalize()
{
    for (const auto& it : m_singletons)
    {
        it.second.registryMetadata->destruct(it.second.component);
        red_free(it.second.component);
    }

    for (const auto& poolIt : m_pools)
    {
        if (!poolIt->registryMetadata->traits.triviallyDestructible)
        {
            for (auto compIt : poolIt->components)
            {
                poolIt.second.registryMetadata->destruct(compIt.second);
            }
        }
    }
}

ComponentVoidPtr ComponentManager::CreateComponent(const TypeTraits& componentTraits, EntityId entityId)
{
    auto it = m_pools.find(componentTraits);
    if (it == m_pools.end())
    {
        RED_LOG_ERROR("Failed to create a component of type {}.", componentTraits.name);
        return nullptr;
    }

    ComponentPool& pool = it->second;
    auto entityCompIt = pool.components.find(entityId);

    if (entityCompIt != pool.components.end())
    {
        RED_LOG_ERROR("Entity {} already have component of type {}", entityId, componentTraits.name);
        return entityCompIt->second;
    }

    ComponentVoidPtr ptr = pool.allocator.AllocateElement();

    if (!pool.registryMetadata->traits.triviallyConstructible)
    {
        pool.registryMetadata->construct(ptr);
    }

    pool.components.insert({entityId, ptr});

    return ptr;
}

void ComponentManager::RemoveComponent(const TypeTraits& componentTraits, EntityId entityId)
{
    auto it = m_pools.find(componentTraits);
    if (it == m_pools.end())
    {
        RED_LOG_ERROR("Failed to remove a component of type {}.", componentTraits.name);
        return;
    }

    ComponentPool& pool = it->second;
    auto entityCompIt = pool.components.find(entityId);

    if (entityCompIt == pool.components.end())
    {
        return;
    }

    auto ptr = entityCompIt->second;

    if (!pool.registryMetadata->traits.triviallyDestructible)
    {
        pool.registryMetadata->destruct(ptr);
    }

    pool.components.erase(entityCompIt);
}

}  // namespace red