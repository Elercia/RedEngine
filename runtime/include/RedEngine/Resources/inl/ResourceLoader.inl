#include "../ResourceLoader.hpp"

namespace red
{
template <typename ResourceType>
ResourceHolder<ResourceType> ResourceListHolderComponent<ResourceType>::GetFromCache(const ResourceId& path)
{
    for(auto holder : m_resources)
    {
        if(holder->GetResourceId() == path)
            return holder;
    }

    return nullptr;
}

template <typename ResourceType>
ResourceHolder<ResourceType> ResourceListHolderComponent<ResourceType>::GetOrCreateFromCache(const ResourceId& path)
{
    auto holder = GetFromCache(path);

    if(holder == nullptr)
    {
        holder = std::make_shared<ResourceType>(path);
        m_resources.push_back(holder);
    }

    return holder;
}

template <typename ResourceType, typename ... OtherQueries>
void ResourceLoader<ResourceType, OtherQueries...>::Update()
{
    auto resourceHolder = std::get<ResourceListHolderComponent<ResourceType>*>(this->m_query.GetSingletonComponents());
    auto requestComponents = this->m_query.GetEntitiesComponents();

    for(auto holder : resourceHolder->m_resources)
    {
        if(holder.use_count() == 1)
        {
            FinalizeResource(holder, std::get<OtherQueries*>(this->m_query.GetSingletonComponents())...);
        }
    }

    for(auto& requestTuple : requestComponents)
    {
        const auto* requestComp = std::get<ResourceRequestComponent*>(requestTuple);

        auto& resourceRequests = requestComp->m_requests; // TODO request can be for another loader

        for( auto requestResourceId : resourceRequests)
        {
            ResourceHolder<ResourceType> cachedResource = resourceHolder->GetOrCreateFromCache(requestResourceId);

            if (cachedResource->GetStatus() == ResourceStatus::Loaded ||cachedResource->GetStatus() == ResourceStatus::Error)
                continue;

            cachedResource->SetStatus(ResourceStatus::Error);

            if (!requestResourceId.Exist() || requestResourceId.IsDirectory())
            {
                RED_LOG_WARNING("Cannot load resource of type {} from path {}", TypeInfo<ResourceType>().name,
                                requestResourceId.GetAscciiPath());
                continue;
            }

            auto fileContent = ReadFile(requestResourceId);
            auto parsedJson = Json::parse(fileContent.begin(), fileContent.end(), nullptr, false, true);

            if (InitResource(cachedResource, requestResourceId, parsedJson, std::get<OtherQueries*>(this->m_query.GetSingletonComponents())...))// TODO pass other queries
            {
                cachedResource->SetStatus(ResourceStatus::Loaded);
            }

            RED_LOG_TRACE("Creating {} from path {}", TypeInfo<ResourceType>().name, requestResourceId.GetAscciiPath());
        }
        //TODO How to remove the request component
    }
}
}