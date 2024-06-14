#pragma once

#include "RedEngine/Audio/AudioEvent.hpp"

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
    AudioSource(const SoundDesc& desc);
    ~AudioSource() = default;

    void Play();
    void Stop();
    void Pause();
    void UnPause();
    void Loop(int loopCount = -1);

    SoundDesc m_desc;

    std::shared_ptr<SoundResource> m_sound = nullptr;
    FMOD::Channel* m_currentChannel = nullptr;

    bool m_needStart = false;
    bool m_needStop = false;
    bool m_needPause = false;
    bool m_needUnpause = false;
};

}  // namespace red