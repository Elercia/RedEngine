#pragma once

#include "RedEngine/Resources/ResourceLoader.hpp"

namespace FMOD
{
class Sound;
};

namespace red
{
class SoundResource : public Resource
{
public:
    SoundResource(const ResourceId& path);
    ~SoundResource();

    FMOD::Sound* GetSound();
    void SetSound(FMOD::Sound* sound);

private:
    FMOD::Sound* m_sound;
};
RED_RESOURCE(SoundResource)
}  // namespace red