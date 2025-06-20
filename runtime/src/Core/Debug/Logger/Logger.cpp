#include "RedEngine/Core/Debug/Logger/Logger.hpp"

#include "RedEngine/Core/Debug/DebugModule.hpp"

#include "RedEngine/Core/SerializationFunctions.hpp"

#include <fmt/core.h>

namespace red
{
const String Logger::logLevelAsString[] = {
    "CUSTOM", "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL",
};

void SetLogLevel(LogLevel level)
{
    GetRedLogger()->SetLogLevel(level);
}

Logger::Logger() : m_logLevel(LogLevel::LEVEL_INFO)
{
}

void Logger::SetLogLevel(LogLevel level)
{
    m_logLevel = level;
}

LogLevel Logger::GetLogLevel() const
{
    return m_logLevel;
}

void Logger::Out(const LogOoutputInfo& data)
{
    m_delegates(data);
}

Logger::OutputDelegate::FuncIndex Logger::AddOutput(OutputDelegate::FuncType output)
{
    return m_delegates.Add(std::move(output));
}

void Logger::RemoveOutput(Logger::OutputDelegate::FuncIndex index)
{
    m_delegates.Remove(index);
}

void Logger::LogToStandardOutputFun(const LogOoutputInfo& out)
{
    fmt::print("{}\n", out.str);
}

template <>
String Serialize(const LogLevel& value)
{
    return Logger::logLevelAsString[(int)value];
}

template <>
bool Deserialize(LogLevel& value, const String& str)
{
    for (int i = 0; i < 7; i++)
    {
        if (Logger::logLevelAsString[i] == str)
        {
            value = LogLevel(i);
            return true;
        }
    }

    return false;
}

}  // namespace red

red::Logger* GetRedLogger()
{
    static red::Logger logger;

    return &logger;
}
