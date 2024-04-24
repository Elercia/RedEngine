#pragma once

#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"

namespace red
{
class ComponentRegistry;

using ComponentVoidPtr = void*;

class ComponentPool
{
    Map<EntityId, ComponentVoidPtr> components;
};

class SingletonComponent
{
    EntityId entityId;
    ComponentVoidPtr component;
};

class ComponentManager
{
public:
    ComponentManager(ComponentRegistry* registry);

    void CreatePools();


};
}  // namespace red

#include "inl/ComponentManager.inl"