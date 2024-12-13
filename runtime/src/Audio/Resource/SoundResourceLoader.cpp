#include "RedEngine/Audio/Resource/SoundResourceLoader.hpp"

#include "RedEngine/Audio/AudioUtils.hpp"
#include "RedEngine/Audio/System/AudioSystem.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Engine.hpp"
#include "RedEngine/Utils/FileUtils.hpp"

#include <filesystem>
#include <fmod_common.h>

namespace red
{
bool SoundResourceLoader::InitResource(std::shared_ptr<SoundResource>& resource, const Path& /*path*/, const Json& jsonContent)
{
    FMOD::System* fmodSystem = nullptr;  // m_world->GetSystem<AudioSystem>()->GetFmodSystem();

    String soundPath = jsonContent["sound_path"];
    if (soundPath.empty())
    {
        return false;
    }

    FMOD::Sound* fmodSound;

    FmodUtils::FmodCheck(fmodSystem->createSound(soundPath.c_str(), FMOD_2D, nullptr, &fmodSound),
                         "Error creating sound");

    resource->SetSound(fmodSound);

    return true;
}

void SoundResourceLoader::FinalizeResource(std::shared_ptr<SoundResource> resource)
{
    resource->SetStatus(ResourceStatus::Released);
    FmodUtils::FmodCheck(resource->GetSound()->release(), "Error releasing sound");
}

}  // namespace red