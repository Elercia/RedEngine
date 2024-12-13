#pragma once

#include "RedEngine/Audio/AudioEvent.hpp"
#include "RedEngine/Resources/ResourceLoader.hpp"

#include <memory>

namespace FMOD
{
class Channel;
}

namespace red
{
class SoundResource;

struct AudioSource
{
    AudioSource();
    ~AudioSource() = default;

    void Play();
    void Stop();
    void Pause();
    void UnPause();
    void Loop(int loopCount = -1);

    SoundDesc m_desc;

    ResourceHolder<SoundResource> m_sound = nullptr;
    FMOD::Channel* m_currentChannel = nullptr;

    bool m_needStart = false;
    bool m_needStop = false;
    bool m_needPause = false;
    bool m_needUnpause = false;
};

}  // namespace red