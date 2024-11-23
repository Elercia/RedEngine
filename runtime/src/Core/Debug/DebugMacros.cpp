#include "RedEngine/Core/Debug/DebugMacros.hpp"

#include "RedEngine/Core/Debug/DebugModule.hpp"

#include "RedEngine/Core/Debug/Logger/Logger.hpp"

namespace red
{
void LogAssert(const char* filename, int line, const String& s)
{
    GetRedLogger()->LogInternal(LogLevel::LEVEL_ERROR, line, filename, s);
}

static bool shouldBreakOnError = true;

bool ShouldBreakOnError()
{
    return shouldBreakOnError;
}

void SetBreakOnError(bool value)
{
    shouldBreakOnError = value;
}

}  // namespace red