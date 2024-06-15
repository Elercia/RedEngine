#include "EngineTest.hpp"

#include "RedEngine/Core/Configuration/CVar.hpp"
#include "RedEngine/Core/Configuration/CVarManager.hpp"
#include "RedEngine/Utils/SystemInfo.hpp"

#include <catch2/catch.hpp>
#include <windows.h>

namespace red
{
bool EngineTest::Create()
{
    auto cvar = CVarManager::GetInstance().GetFromName<int>("Engine_Thread_ThreadCount");

    cvar.ChangeValue(1);

    m_scheduler.Init();

    InitAllocator();

    return true;
}
bool EngineTest::Destroy()
{
    red_free(m_frameAllocator);

    m_scheduler.Finalize();

    return true;
}

std::string_view EngineTest::GetGameName() const
{
    return "TestEngine";
}

bool EngineTest::RegisterComponentTypes()
{
    return true;
}

void EngineTest::SetupLogger()
{
}
}  // namespace red

static void LogToDebugger(const red::Logger::LogOoutputInfo & out)
{
    OutputDebugStringA(out.str.c_str());
    OutputDebugStringA("\n");
}

int main(int argc, char* argv[])
{
    // your setup ...

    red::InitSystemInfo();

    red::SetBreakOnError(false);

    if (IsDebuggerPresent() != 0)
    {
        GetRedLogger()->AddOutput(LogToDebugger);
    }

    int result = Catch::Session().run(argc, argv);

    // your clean-up...

    return result;
}