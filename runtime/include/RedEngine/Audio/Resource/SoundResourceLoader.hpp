#pragma once

#include "RedEngine/Audio/Resource/SoundResource.hpp"
#include "RedEngine/Resources/ResourceLoader.hpp"

namespace red
{
class world;

class SoundResourceLoader : public ResourceLoader<SoundResource>
{
    void FinalizeResource(std::shared_ptr<SoundResource> resource) override;
    bool InitResource(std::shared_ptr<SoundResource>& resource, const Path& path, const Json& jsonContent) override;
};
} // namespace red