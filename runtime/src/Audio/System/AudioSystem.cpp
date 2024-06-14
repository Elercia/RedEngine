#include "RedEngine/Audio/System/AudioSystem.hpp"

#include "RedEngine/Audio/AudioModule.hpp"

#include "RedEngine/Audio/AudioUtils.hpp"
#include "RedEngine/Audio/Component/AudioListener.hpp"
#include "RedEngine/Audio/Component/AudioSource.hpp"
#include "RedEngine/Audio/Resource/SoundResource.hpp"
#include "RedEngine/Core/Time/Time.hpp"
#include "RedEngine/Entity/World.hpp"

namespace red
{
void AudioInitializer::Init()
{
    auto* subsystem = m_world->GetSingletonComponent<AudioSubSystem>();

    FmodUtils::FmodCheck(FMOD::Studio::System::create(&subsystem->m_studioSystem), "Creating FMOD studio system");
    FmodUtils::FmodCheck(
        subsystem->m_studioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL),
        "Initializing FMOD studio system");
    FmodUtils::FmodCheck(subsystem->m_studioSystem->getCoreSystem(&subsystem->m_system), "Get core system");
}

void AudioInitializer::Finalize()
{
    auto* subsystem = m_world->GetSingletonComponent<AudioSubSystem>();

    FmodUtils::FmodCheck(subsystem->m_studioSystem->unloadAll(), "Unloading");
    FmodUtils::FmodCheck(subsystem->m_studioSystem->release(), "Release");
}

void UpdateAudioSourceSystem::Update()
{
    // Update audio sources
    auto audioSubSystem = std::get<AudioSubSystem*>(m_query.GetSingletonComponents());
    auto audioSources = m_query.GetEntitiesComponents();
    for (auto& sourceTuple : audioSources)
    {
        auto audioSource = std::get<AudioSource*>(sourceTuple);
        auto resource = audioSource->m_sound;

        if (resource->GetLoadState() != LoadState::STATE_LOADED)
        {
            continue;
        }

        if (audioSource->m_needStart)
        {
            audioSubSystem->m_system->playSound(resource->GetSound(), nullptr, false, &audioSource->m_currentChannel);
        }
        audioSource->m_needStart = false;

        if (audioSource->m_currentChannel != nullptr)
        {
            if (audioSource->m_needStop)
            {
                FmodUtils::FmodCheck(audioSource->m_currentChannel->stop(), "failed to stop sound");
            }
            audioSource->m_needStop = false;

            if (audioSource->m_needPause)
            {
                FmodUtils::FmodCheck(audioSource->m_currentChannel->setPaused(true), "failed to pause sound");
            }
            audioSource->m_needPause = false;

            if (audioSource->m_needUnpause)
            {
                FmodUtils::FmodCheck(audioSource->m_currentChannel->setPaused(false), "failed to un-pause sound");
            }
            audioSource->m_needUnpause = false;
        }
    }
}

void UpdateAudioListenerSystem::Update()
{
    auto audioSubSystem = std::get<AudioSubSystem*>(m_query.GetSingletonComponents());

    // Update listeners
    auto audioListeners = m_query.GetEntitiesComponents();

    for (auto& audioTuple : audioListeners)
    {
        const auto* audioListenerTransform = std::get<const Transform2D*>(audioTuple);
        auto audioListener = std::get<AudioListener*>(audioTuple);

        FMOD_VECTOR oldPos = FmodUtils::Convert(audioListener->m_lastFramePos);
        FMOD_VECTOR currentPos = FmodUtils::Convert(audioListenerTransform->localPosition);
        audioListener->m_lastFramePos = audioListenerTransform->localPosition;

        FMOD_VECTOR velocity;
        velocity.x = (currentPos.x - oldPos.x) * (1000 / Time::DeltaTime());
        velocity.y = (currentPos.y - oldPos.y) * (1000 / Time::DeltaTime());
        velocity.z = 0;

        FMOD_VECTOR forward;
        forward.x = 1;
        forward.y = 0;
        forward.z = 0;

        FMOD_VECTOR up;
        up.x = 0;
        up.z = 1;
        up.y = 0;

        FMOD_3D_ATTRIBUTES attributes = {{0}};
        attributes.position = currentPos;
        attributes.velocity = velocity;
        attributes.forward = forward;
        attributes.up = up;
        FmodUtils::FmodCheck(
            audioSubSystem->m_studioSystem->setListenerAttributes(audioListener->m_listenerId, &attributes),
            "Set listener attributes");
    }
}

void UpdateAudioSubSystemSystem::Update()
{
    auto audioSubSystem = std::get<AudioSubSystem*>(m_query.GetSingletonComponents());
    audioSubSystem->m_studioSystem->update();
}

}  // namespace red