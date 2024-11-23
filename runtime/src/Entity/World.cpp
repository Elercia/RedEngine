#include "RedEngine/Entity/World.hpp"

#include "RedEngine/Entity/EntityModule.hpp"

#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/SystemExecutionGraph.hpp"
#include "RedEngine/Entity/Transform.hpp"
#include "RedEngine/Utils/Random.hpp"

namespace red
{
World::World()
    : m_componentRegistry(red_new(ComponentRegistry))
    , m_componentManager(red_new(ComponentManager, m_componentRegistry))
    , m_executionGraph(red_new(SystemExecutionGraph, this))
    , m_iCurrentEntityId(0)
{
}

World::~World()
{
    red_delete(m_executionGraph);
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
    m_executionGraph->Execute();
}

SystemExecutionGraph* World::GetExecutionGraph()
{
    return m_executionGraph;
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

EntityId World::CreateEntity()
{
    m_iCurrentEntityId++;

    return m_iCurrentEntityId;
}

}  // namespace red