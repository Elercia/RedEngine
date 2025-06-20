
#include "RedEngine/Core/Debug/DebugMacros.hpp"
#include "RedEngine/Core/SerializationFunctions.hpp"
#include "RedEngine/Math/Matrix.hpp"
#include "RedEngine/Math/Vector.hpp"
#include "RedEngine/Utils/StringUtils.hpp"

using namespace red::utils;  // StringReader

namespace red
{
template <>
String Serialize(const Vector2& value)
{
    return fmt::format("{};{}", value.x, value.y);
}

template <>
bool Deserialize(Vector2& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadFloat(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.y));

    return true;
}

template <>
String Serialize(const Vector2i& value)
{
    return fmt::format("{};{}", value.x, value.y);
}

template <>
bool Deserialize(Vector2i& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadInt(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.y));

    return true;
}

template <>
String Serialize(const Vector3& value)
{
    return fmt::format("{};{};{}", value.x, value.y, value.z);
}

template <>
bool Deserialize(Vector3& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadFloat(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.y));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.z));

    return true;
}

template <>
String Serialize(const Vector3i& value)
{
    return fmt::format("{};{};{}", value.x, value.y, value.z);
}

template <>
bool Deserialize(Vector3i& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadInt(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.y));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.z));

    return true;
}

template <>
String Serialize(const Vector4& value)
{
    return fmt::format("{};{};{};{}", value.x, value.y, value.z, value.w);
}

template <>
bool Deserialize(Vector4& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadFloat(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.y));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.z));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadFloat(&it, end, value.w));

    return true;
}

template <>
String Serialize(const Vector4i& value)
{
    return fmt::format("{};{};{};{}", value.x, value.y, value.z, value.w);
}

template <>
bool Deserialize(Vector4i& value, const String& str)
{
    const char* it = str.data();
    const char* end = (&str.back()) + 1;

    CheckReturn(StringParser::ReadInt(&it, end, value.x));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.y));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.z));
    CheckReturn(StringParser::Expect(&it, end, ';'));
    CheckReturn(StringParser::ReadInt(&it, end, value.w));

    return true;
}

}  // namespace red