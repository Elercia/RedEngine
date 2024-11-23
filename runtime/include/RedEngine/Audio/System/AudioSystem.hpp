#pragma once

#include "RedEngine/Audio/Component/AudioListener.hpp"
#include "RedEngine/Audio/Component/AudioSource.hpp"
#include "RedEngine/Audio/Component/AudioSubSystem.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/Transform.hpp"

#include <fmod.hpp>
#include <fmod_studio.hpp>

namespace red
{

class AudioInitializer : public SystemInitializer
{
    virtual void Init() override;
    virtual void Finalize() override;
};

using UpdateAudioSourceSystemQuery = Query<Reading<Transform2D>, Writing<AudioSource>, Writing<AudioSubSystem>>;
class UpdateAudioSourceSystem : public System<UpdateAudioSourceSystemQuery>
{
public:
    virtual void Update() override;
};

using UpdateAudioListenerSystemQuery = Query<Reading<Transform2D>, Writing<AudioListener>, Writing<AudioSubSystem>>;
class UpdateAudioListenerSystem : public System<UpdateAudioListenerSystemQuery>
{
public:
    virtual void Update() override;
};

using UpdateAudioSubSystemQuery = Query<Writing<AudioSubSystem>>;
class UpdateAudioSubSystemSystem : public System<UpdateAudioSubSystemQuery>
{
public:
    virtual void Update() override;
};

}  // namespace red