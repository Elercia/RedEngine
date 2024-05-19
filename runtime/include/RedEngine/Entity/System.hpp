#pragma once

#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Entity/SystemQuery.hpp"
#include "RedEngine/Utils/Tuple.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{

template <typename QueryType>
struct System
{
    static_assert(std::is_base_of<Query, QueryType>::value, "Not a query type");

    virtual void Init(){};
    virtual void Finalize(){};
    virtual void Update(){};
};
}  // namespace red
#include "inl/System.inl"