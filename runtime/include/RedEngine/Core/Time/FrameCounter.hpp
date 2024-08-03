#pragma once

#include <array>
#include <chrono>

#include "RedEngine/Utils/Types.hpp"

namespace red
{
constexpr uint8_t FrameCounterBufferSize = 10;

class FrameCounter
{
    using Clock = std::chrono::system_clock;

public:
    FrameCounter();
    ~FrameCounter() = default;

    float Update();

    uint64 GetFrameIndex() const;

private:
    Clock::time_point _lastFrameTimePoint;
    std::array<float, FrameCounterBufferSize> m_frameBuffer;
    uint64 m_currentFrameBufferIndex = 0;
};
}  // namespace red
