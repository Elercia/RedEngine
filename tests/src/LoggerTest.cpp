#include "RedEngine/Core/Debug/Logger/Logger.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Logger", "[Debug]")
{
    using namespace red;

    Logger logger;

    std::string lastLog;

    auto outFunction = [&](const Logger::LogOoutputInfo& out) { lastLog = out.str; };

    logger.AddOutput(outFunction);

    SECTION("Logging trace")
    {
        logger.SetLogLevel(red::LogLevel::LEVEL_TRACE);
        logger.LogInternal(red::LogLevel::LEVEL_TRACE, 10, "LoggerTest.cpp", "Doing stuff with {}, {} times",
                           "my bicycle", 10);

        REQUIRE(lastLog.find("Doing stuff with my bicycle, 10 times") != std::string::npos);
        REQUIRE(lastLog.find("TRACE") != std::string::npos);
    }
}