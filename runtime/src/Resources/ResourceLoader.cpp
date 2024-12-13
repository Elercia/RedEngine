#include "RedEngine/Resources/ResourceLoader.hpp"

namespace red
{
Resource::Resource(const ResourceId& id)
    : m_id(id)
    , m_satus(ResourceStatus::NotLoaded)
{
}

ResourceStatus Resource::GetStatus() const
{
    return m_satus;
}

void Resource::SetStatus(ResourceStatus status)
{
    m_satus = status;
}

const ResourceId& Resource::GetResourceId() const
{
    return m_id;
}
}