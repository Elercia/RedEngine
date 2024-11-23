#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace red
{
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using sizet = std::size_t;

constexpr int8 RED_INT8_MAX = std::numeric_limits<int8>::max();
constexpr int16 RED_INT16_MAX = std::numeric_limits<int16>::max();
constexpr int32 RED_INT32_MAX = std::numeric_limits<int32>::max();
constexpr int64 RED_INT64_MAX = std::numeric_limits<int64>::max();

constexpr uint8 RED_UINT8_MAX = std::numeric_limits<uint8>::max();
constexpr uint16 RED_UINT16_MAX = std::numeric_limits<uint16>::max();
constexpr uint32 RED_UINT32_MAX = std::numeric_limits<uint32>::max();
constexpr uint64 RED_UINT64_MAX = std::numeric_limits<uint64>::max();
}  // namespace red
