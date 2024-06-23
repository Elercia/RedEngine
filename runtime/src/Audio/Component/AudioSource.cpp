#include "RedEngine/Audio/Component/AudioSource.hpp"

#include "RedEngine/Audio/AudioModule.hpp"

#include "RedEngine/Audio/Resource/SoundResourceLoader.hpp"
#include "RedEngine/Resources/ResourceHolderComponent.hpp"

namespace red
{
AudioSource::AudioSource() : m_desc(), m_currentChannel(nullptr)
{
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
