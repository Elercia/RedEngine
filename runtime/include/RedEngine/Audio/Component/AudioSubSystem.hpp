#pragma once

#include "RedEngine/Entity/ComponentRegistry.hpp"

namespace FMOD
{
namespace Studio
{
    class System;
}

class System;
}  // namespace FMOD

namespace red
{
struct AudioSubSystem
{
    FMOD::Studio::System* m_studioSystem;
    FMOD::System* m_system;
};

RED_DECLARE_SINGLETON_COMPONENT(AudioSubSystem)

}  // namespace red