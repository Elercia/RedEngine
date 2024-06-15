#pragma once

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/ComponentManager.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"

namespace red
{
class BaseSystem;
class SystemInitializer;

class World : Uncopyable, Unmovable
{
public:
    World();
    ~World();

    void Init();
    void Finalize();

    void Update();

    template <typename T>
    T* AddSystem();
    template <typename T>
    void RemoveSystem();

    template <typename T>
    void RegisterComponent();

    template <typename T>
    T* AddSystemInitializer();
    template <typename T>
    void RemoveSystemInitializer();

    template <typename T>
    T* GetSingletonComponent();

    template <typename TupleType>
    void QuerySingletons(TupleType& tuple);

    template <typename TupleType>
    void QueryComponents(Array<TupleType>& tuples);

    EntityId CreateEntity();

    template <typename COMP>
    COMP* AddComponentToEntity(EntityId id);

private:
    void AddSystem(BaseSystem* system, const TypeTraits& traits);
    void RemoveSystem(const TypeTraits& traits);

    void AddSystemInitializer(SystemInitializer* systemInitializer, const TypeTraits& traits);
    void RemoveSystemInitializer(const TypeTraits& traits);

private:
    ComponentRegistry* m_componentRegistry;
    ComponentManager* m_componentManager;

    Array<BaseSystem*> m_systems;
    Array<SystemInitializer*> m_systemInitializers;

    EntityId m_iCurrentEntityId;
};
}  // namespace red

#include "inl/World.inl"