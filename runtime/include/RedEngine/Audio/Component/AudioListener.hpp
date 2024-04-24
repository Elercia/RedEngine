#pragma once

#include "RedEngine/Math/Vector.hpp"

#include "fmod_common.h"

namespace red
{
struct AudioListener
{
    int m_listenerId;
    Vector2 m_lastFramePos;
};

}  // namespace red