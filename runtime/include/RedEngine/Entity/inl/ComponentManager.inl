namespace red
{
template <typename COMP>
COMP* ComponentManager::CreateComponent(EntityId entityId)
{
    return (COMP*)CreateComponent(TypeInfo<COMP>(), entityId);
}

template <typename... COMPS>
Array<Tuple<EntityId, COMPS*...>> ComponentManager::GetComponents()
{
    using CompsTupleType = Tuple<COMPS*...>;
    constexpr auto CompsTupleTypeSize = TupleSize<CompsTupleType>::value;

    static_assert(CompsTupleTypeSize != 0, "You must call get component with at least one component");

    ComponentPool* concernedPools[CompsTupleTypeSize] = {GetPool<COMPS>()...};

    Array<Tuple<EntityId, COMPS*...>> outResult;

    Intersect(outResult, ArrayView(concernedPools, CompsTupleTypeSize));

    return outResult;
}

template <typename COMP>
void ComponentManager::RemoveComponent(EntityId entityId)
{
    RemoveComponent(TypeInfo<COMP>(), entityId);
}

template <typename SINGL>
inline SINGL* ComponentManager::GetSingletonComponent()
{
    static_assert(std::is_pointer_v<SINGL> == false && std::is_const_v<SINGL> == false &&
                  std::is_reference_v<SINGL> == false);

    auto typeTraits = TypeInfo<SINGL>();

    auto it = m_singletons.find(typeTraits);
    if (it == m_singletons.end())
        return nullptr;

    return (SINGL*)it->second.component;
}

template <typename POOL>
ComponentPool* ComponentManager::GetPool()
{
    auto typeTraits = TypeInfo<POOL>();

    auto it = m_pools.find(typeTraits);
    if (it == m_pools.end())
    {
        RED_LOG_ERROR("Failed to get component pool for type {} (missing register call ?)", typeTraits.name);
        return nullptr;
    }

    return &it->second;
}
template <typename... COMPS>
inline void ComponentManager::Intersect(Array<Tuple<EntityId, COMPS*...>>& outResult, ArrayView<ComponentPool*> pools)
{
    using EntityAndCompsTupleType = Tuple<EntityId, COMPS*...>;
    using CompsTupleType = Tuple<COMPS*...>;
    constexpr auto CompsTupleTypeSize = TupleSize<CompsTupleType>::value;

    // Find the pool with the least component inside
    // This will give us our highest bound of entities
    // For each other pools, search entities from the first.
    ComponentPool* smallestPool = nullptr;
    for (uint32 i = 0; i < pools.size(); i++)
    {
        if (smallestPool == nullptr || smallestPool->components.size() > pools[i]->components.size())
        {
            smallestPool = pools[i];
        }
    }

    for (auto compIt : smallestPool->components)
    {
        bool foundAllComponents = true;

        EntityId entityToSearchFor = compIt.first;

        std::array<ComponentVoidPtr, CompsTupleTypeSize> componentsList;

        for (uint32 i = 0; i < pools.size(); i++)
        {
            auto pool = pools[i];

            if (pool != smallestPool)
            {
                auto otherComp = pool->components.find(entityToSearchFor);
                if (otherComp == pool->components.end())
                {
                    foundAllComponents = false;
                    break;
                }
                else
                {
                    componentsList[i] = otherComp->second;
                }
            }
            else
            {
                componentsList[i] = compIt.second;
            }
        }

        if (foundAllComponents)
        {
            Tuple<EntityId> entityTuple{entityToSearchFor};
            auto componentVoidTuple = std::tuple_cat(componentsList);
            CompsTupleType casted;
            cast_tuple(componentVoidTuple, casted);

            auto fullTuple = std::tuple_cat(entityTuple, casted);
            outResult.push_back(fullTuple);
        }
    }
}
}  // namespace red