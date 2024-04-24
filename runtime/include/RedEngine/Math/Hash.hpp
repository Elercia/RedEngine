#pragma once

#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Utils/Types.hpp"

namespace red
{
constexpr uint32 fnv1a32(const StringView& s, sizet count);
constexpr uint32 fnv1a32(const StringView& s);

constexpr uint32 wfnv1a32(const WStringView& s, sizet count);
constexpr uint32 wfnv1a32(const WStringView& s);

template <typename T>
inline uint64 Hash(const T& value);

template <>
inline uint64 Hash(const int& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const uint64& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const uint32& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const double& value)
{
    return Hash((uint64)value);
}

template <>
inline uint64 Hash(const float& value)
{
    return Hash((uint32)value);
}

template <>
inline uint64 Hash(const uint8& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const uint16& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const int8& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const int16& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const int64& value)
{
    return (uint64)value;
}

template <>
inline uint64 Hash(const String& value)
{
    return fnv1a32(value);
}

template <>
inline uint64 Hash(const StringView& value)
{
    return fnv1a32(value);
}

template <>
inline uint64 Hash( char* const& value)
{
    return fnv1a32(value);
}
}  // namespace red

#include "inl/Hash.inl"