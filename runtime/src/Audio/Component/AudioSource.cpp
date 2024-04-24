#include "RedEngine/Audio/Component/AudioSource.hpp"

#include "RedEngine/Audio/AudioModule.hpp"

#include "RedEngine/Audio/Resource/SoundResourceLoader.hpp"
#include "RedEngine/Resources/ResourceHolderComponent.hpp"

namespace red
{
AudioSource::AudioSource(const SoundDesc& desc) : m_desc(desc), m_currentChannel(nullptr)
{
    /*m_sound = m_owner->GetWorld()
                  ->GetWorldComponent<ResourceHolderComponent>()
                  ->GetResourceLoader<SoundResourceLoader>()
                  ->LoadResource(Path::Resource(desc.name));*/
}

void AudioSource::Play()
{
    m_needStart = true;
    m_needStop = false;
}

void AudioSource::Stop()
{
    m_needStart = false;
    m_needStop = true;
}

}  // namespace red
