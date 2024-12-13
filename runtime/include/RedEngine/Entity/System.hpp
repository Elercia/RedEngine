#pragma once

#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Entity/SystemQuery.hpp"
#include "RedEngine/Utils/Tuple.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{
class World;

// TODO Query ?
class SystemInitializer
{
public:
    virtual void Init() = 0;
    virtual void Finalize() = 0;

    World* m_world;
    TypeTraits m_traits;
};

class BaseSystem
{
public:
    virtual ~BaseSystem() = default;
    
    virtual void Update() = 0;
    virtual BaseQuery* GetQuery() = 0;
    World* m_world;
    TypeTraits m_traits;
};

template <typename QueryType>
class System : public BaseSystem
{
public:
    static_assert(std::is_base_of<BaseQuery, QueryType>::value, "Not a query type");

    QueryType m_query;

    virtual BaseQuery* GetQuery()
    {
        return &m_query;
    };
};
}  // namespace red
#include "inl/System.inl"