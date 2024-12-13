#pragma once

#include "RedEngine/Resources/ResourceModule.hpp"

#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Filesystem/Path.hpp"
#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Utils/FileUtils.hpp"

#include <nlohmann/json.hpp>

namespace red
{
using ResourceId = Path;
using Json = nlohmann::json;

enum class ResourceStatus : uint8
{
    NotLoaded,
    Loaded,
    Error,
    Released
};

class Resource : public red::Uncopyable
{
public:
    explicit Resource(const ResourceId& id);

    virtual ~Resource() = default;
    Resource(Resource&& other) = default;
    Resource& operator=(Resource&& other) = default;

    ResourceStatus GetStatus() const;
    void SetStatus(ResourceStatus status);

    const ResourceId& GetResourceId() const;

private:
    ResourceId m_id;
    ResourceStatus m_satus;
};

class GenericResourceHandle
{
    
};

template <typename T>
class ResourceHandle : public GenericResourceHandle
{
};

class IResourceManager : Uncopyable, Unmovable
{
public:
    IResourceManager() = default;
    virtual ~IResourceManager() = default;

    virtual void Init();
    virtual void Finalize();

    virtual GenericResourceHandle GetResource(const ResourceId& id) = 0; // Immediate load
    virtual GenericResourceHandle LoadResource(const ResourceId& id) = 0; // Push a load command
};

template <typename ResourceType>
class ResourceManager : public IResourceManager
{
public:
    ResourceManager() = default;
    virtual ~ResourceManager() = default;

    virtual GenericResourceHandle LoadResource(const ResourceId& id) override;
};

}

#include "inl/ResourceLoader.inl"