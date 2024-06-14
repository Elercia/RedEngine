#include "RedEngine/Entity/World.hpp"

#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/Transform.hpp"

namespace red
{
World::World()
    : m_componentRegistry(red_new(ComponentRegistry))
    , m_componentManager(red_new(ComponentManager, m_componentRegistry))
{
}

World::~World()
{
    red_delete(m_componentManager);
    red_delete(m_componentRegistry);
}

void World::Init()
{
    RegisterComponent<Transform2D>();

    m_componentManager->Init();

    for (auto* initializer : m_systemInitializers)
    {
        initializer->Init();
    }
}

void World::Finalize()
{
    for (auto* initializer : m_systemInitializers)
    {
        initializer->Finalize();
    }

    m_componentManager->Finalize();
}

void World::Update()
{
    for (auto system : m_systems)
    {
        system->Update();
    }
}

void World::AddSystem(BaseSystem* system, const TypeTraits& traits)
{
    system->m_world = this;
    system->m_traits = traits;

    m_systems.push_back(system);
}

void World::RemoveSystem(const TypeTraits& traits)
{
    for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
    {
        if ((*it)->m_traits == traits)
        {
            m_systems.erase(it);
            break;
        }
    }
}

void World::AddSystemInitializer(SystemInitializer* systemInitializer, const TypeTraits& traits)
{
    systemInitializer->m_world = this;
    systemInitializer->m_traits = traits;

    m_systemInitializers.push_back(systemInitializer);
}

void World::RemoveSystemInitializer(const TypeTraits& traits)
{
    for (auto it = m_systemInitializers.begin(); it != m_systemInitializers.end(); ++it)
    {
        if ((*it)->m_traits == traits)
        {
            m_systemInitializers.erase(it);
            break;
        }
    }
}
}  // namespace red