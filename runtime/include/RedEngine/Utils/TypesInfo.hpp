#pragma once

#include "RedEngine/Math/Hash.hpp"

#include "RedEngine/Core/Container/String.hpp"
#include <tuple>

namespace red
{
using TypeTraitsId = uint32;

struct TypeTraits
{
    StringView name;
    TypeTraitsId typeId;
    uint64 size;
    bool triviallyConstructible;
    bool triviallyDestructible;

    constexpr bool operator==(const TypeTraits& other) const
    {
        return other.typeId == typeId;
    }

    constexpr bool operator!=(const TypeTraits& other) const
    {
        return other.typeId != typeId;
    }
};

static_assert(std::is_trivially_destructible<TypeTraits>::value, "TypeTraits must be trivially_destructible");

constexpr TypeTraits EmptyTypeTraits{"invalid", (uint32) -1};

#if defined(_MSC_VER) && !defined(__clang__)
#define RED_FUNCNAME              __FUNCSIG__
#define RED_FUNCNAME_START        "red::TypeInfo<"
#define RED_FUNCNAME_END          ">(void)"
#define RED_FUNCNAME_STRUCT_START "struct "
#define RED_FUNCNAME_CLASS_START  "class "
#elif defined(__clang__)
#define RED_FUNCNAME              __PRETTY_FUNCTION__
#define RED_FUNCNAME_START        "red::TypeInfo() [T ="
#define RED_FUNCNAME_END          "]"
#define RED_FUNCNAME_STRUCT_START ""
#define RED_FUNCNAME_CLASS_START  ""
#elif defined(__GNUC__)
#define RED_FUNCNAME              __PRETTY_FUNCTION__
#define RED_FUNCNAME_START        "red::TypeInfo() [with T = "
#define RED_FUNCNAME_END          "]"
#define RED_FUNCNAME_STRUCT_START "struct "
#define RED_FUNCNAME_CLASS_START  "class "
#endif

#define RED_TYPE_INFO(var) red::TypeInfo<decltype(var)>()

template <typename T>
constexpr TypeTraits TypeInfo();

inline TypeTraits GetTypeInfoFromTypeName(StringView name);
}  // namespace red

#include "inl/TypesInfo.inl"