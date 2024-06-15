namespace red
{
template <typename T>
T* World::AddSystem()
{
    static_assert(std::is_base_of<BaseSystem, T>::value, "AddSystem called with a type that is not a system");

    T* system = red_new(T);
    AddSystem(system, TypeInfo<T>());

    system->m_query.m_world = this;

    return system;
}

template <typename T>
void World::RemoveSystem()
{
    static_assert(std::is_base_of<BaseSystem, T>::value, "RemoveSystem called with a type that is not a system");

    constexpr auto typeTraits = TypeInfo<T>();

    RemoveSystem(typeTraits);
}

template <typename T>
void World::RegisterComponent()
{
    m_componentRegistry->RegisterComponent<T>();
}

template <typename T>
T* World::AddSystemInitializer()
{
    static_assert(std::is_base_of<SystemInitializer, T>::value,
                  "AddSystemInitializer called with a type that is not a SystemInitializer");

    T* systemInitializer = red_new(T);

    AddSystemInitializer(systemInitializer, TypeInfo<T>());

    return systemInitializer;
}

template <typename T>
void World::RemoveSystemInitializer()
{
    static_assert(std::is_base_of<SystemInitializer, T>::value,
                  "RemoveSystemInitializer called with a type that is not a SystemInitializer");

    constexpr auto typeTraits = TypeInfo<T>();

    RemoveSystemInitializer(typeTraits);
}

template <typename T>
T* World::GetSingletonComponent()
{
    return m_componentManager->GetSingletonComponent<T>();
}

template <typename TupleType>
void World::QuerySingletons(TupleType& tuple)
{
    for_each(tuple,
             [=](int /*index*/, auto& elem)
             {
                 elem = m_componentManager->GetSingletonComponent<
                     std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<decltype(elem)>>>>();
             });
}

template <typename TupleType>
void World::QueryComponents(Array<TupleType>& tuples)
{
    m_componentManager->GetComponents(tuples);
}

template <typename COMP>
COMP* World::AddComponentToEntity(EntityId id)
{
    return m_componentManager->CreateComponent<COMP>(id);
}
}  // namespace red