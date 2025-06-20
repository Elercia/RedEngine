#pragma once

#include "RedEngine/Core/Container/String.hpp"
#include "RedEngine/Core/Event/Delegate.hpp"

namespace red
{

enum class LogLevel
{
    LEVEL_CUSTOM,
    LEVEL_TRACE,
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_FATAL
};

void SetLogLevel(LogLevel level);

class Logger
{
public:
    struct LogOoutputInfo
    {
        String str;
        LogLevel level;
    };

    static const String logLevelAsString[7];

    using OutputDelegate = Delegate<const LogOoutputInfo&>;

    Logger();
    ~Logger() = default;

    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const;

    template <typename... Args>
    void LogInternal(LogLevel level, int line, const char* file, const StringView& format, Args... args);

    void Out(const LogOoutputInfo& data);

    OutputDelegate::FuncIndex AddOutput(OutputDelegate::FuncType output);
    void RemoveOutput(OutputDelegate::FuncIndex index);

    static void LogToStandardOutputFun(const LogOoutputInfo& out);

private:
    LogLevel m_logLevel;
    OutputDelegate m_delegates;
};

}  // namespace red

red::Logger* GetRedLogger();

#define RED_LOG(LEVEL, MSG, ...) GetRedLogger()->LogInternal(LEVEL, __LINE__, __FILE__, MSG, ##__VA_ARGS__)

#define RED_LOG_TRACE(MSG, ...)   RED_LOG(LogLevel::LEVEL_TRACE, MSG, ##__VA_ARGS__)
#define RED_LOG_DEBUG(MSG, ...)   RED_LOG(LogLevel::LEVEL_DEBUG, MSG, ##__VA_ARGS__)
#define RED_LOG_INFO(MSG, ...)    RED_LOG(LogLevel::LEVEL_INFO, MSG, ##__VA_ARGS__)
#define RED_LOG_WARNING(MSG, ...) RED_LOG(LogLevel::LEVEL_WARNING, MSG, ##__VA_ARGS__)
#define RED_LOG_ERROR(MSG, ...)                         \
    RED_LOG(LogLevel::LEVEL_ERROR, MSG, ##__VA_ARGS__); \
    RedError()
#define RED_LOG_FATAL(MSG, ...)                         \
    RED_LOG(LogLevel::LEVEL_FATAL, MSG, ##__VA_ARGS__); \
    RedError()

#include "inl/Logger.inl"
