#include "RedEngine/Core/Engine.hpp"

#include "RedEngine/Core/CoreModule.hpp"

#include "RedEngine/Audio/Component/AudioListener.hpp"
#include "RedEngine/Audio/Component/AudioSource.hpp"
#include "RedEngine/Audio/Component/AudioSubSystem.hpp"
#include "RedEngine/Audio/Resource/SoundResourceLoader.hpp"
#include "RedEngine/Audio/System/AudioSystem.hpp"
#include "RedEngine/Core/Configuration/CVar.hpp"
#include "RedEngine/Core/Configuration/CVarManager.hpp"
#include "RedEngine/Core/Debug/Component/DebugComponent.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Debug/System/DebugSystem.hpp"
#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Core/Event/System/EventSystem.hpp"
#include "RedEngine/Core/Time/FrameCounter.hpp"
#include "RedEngine/Core/Time/Time.hpp"
#include "RedEngine/Entity/SystemExecutionGraph.hpp"
#include "RedEngine/Entity/Transform.hpp"
#include "RedEngine/Entity/World.hpp"
#include "RedEngine/Input/Component/UserInput.hpp"
#include "RedEngine/Input/System/UserInputSystem.hpp"
#include "RedEngine/Physics/Components/PhysicBody.hpp"
#include "RedEngine/Physics/Components/PhysicsWorld.hpp"
#include "RedEngine/Physics/System/PhysicsSystem.hpp"
#include "RedEngine/Rendering/Renderer.hpp"
#include "RedEngine/Resources/ResourceHolderComponent.hpp"
#include "RedEngine/Utils/Random.hpp"
#include "RedEngine/Utils/SystemInfo.hpp"

#include "RedEngine/Core/Debug/Profiler.hpp"


#ifdef RED_WINDOWS
#define NOMINMAX 1
#include <windows.h>
#endif

namespace red
{
#ifdef RED_DEBUG
#define DefaultLogLevel LogLevel::LEVEL_DEBUG
#else
#define DefaultLogLevel LogLevel::LEVEL_INFO
#endif

static CVar<bool> s_addStandardOutputLog{"AddStandardOutputlog", "Logger", false};
static CVar<LogLevel> s_logLevel{"LogLevel", "Logger", DefaultLogLevel};

Engine* Engine::s_engine = nullptr;

Engine* Engine::GetInstance()
{
    return s_engine;
}

Engine::Engine()
    : m_argc(0)
    , m_argv(nullptr)
    , m_world(nullptr)
    , m_renderer(nullptr)
    , m_frameAllocator(nullptr)
{
}

Engine::~Engine()
{
}

void Engine::MainLoop()
{
    bool continueExec = true;

    FrameCounter fc;

    while (continueExec)
    {
        PROFILER_FRAME("MainThread");

        float deltaTime = fc.Update();

        Time::SetDeltaTime(deltaTime);

        // continueExec = m_world->Update();

        for (int i = 0; i < m_scheduler.GetWorkerCount(); i++)
        {
            m_frameAllocator[i].Swap();
        }
    }
}

bool Engine::RegisterComponentTypes()
{
    PROFILER_EVENT();

    m_world->RegisterComponent<Transform2D>();

    // Debug
    m_world->RegisterComponent<DebugComponent>();

    // Inputs
    m_world->RegisterComponent<EventsComponent>();
    m_world->RegisterComponent<UserInputComponent>();

    // Physics
    m_world->RegisterComponent<PhysicsWorld>();
    m_world->RegisterComponent<PhysicBody>();

    // Audio
    m_world->RegisterComponent<AudioSubSystem>();
    m_world->RegisterComponent<AudioSource>();
    m_world->RegisterComponent<AudioListener>();

    return true;
}

bool Engine::RegisterSystems()
{
    auto* graph = m_world->GetExecutionGraph();

    // Audio
    m_world->AddSystemInitializer<AudioInitializer>();

    graph->AddSystem<UpdateAudioSourceSystem>().Before<UpdateAudioSubSystemSystem>();
    graph->AddSystem<UpdateAudioListenerSystem>().Before<UpdateAudioSubSystemSystem>();
    graph->AddSystem<UpdateAudioSubSystemSystem>();

#ifdef RED_DEVBUILD
    // Debug
    m_world->AddSystemInitializer<DebugSystemInitializer>();
    graph->AddSystem<DebugSystem>();
#endif

    // Input
    // TODO Add system priority to make it the first thing to do in the frame ?
    //  Or other system have to put it in dependancy ?
    graph->AddSystem<EventSystem>();
    graph->AddSystem<UserInputSystem>().After<EventSystem>();

    // Physics
    // TODO add system initializer for physics (create the B2 world)
    graph->AddSystem<UpdatePhysicsFromEntitiesSystem>().Before<UpdatePhysicSystem>();
    graph->AddSystem<UpdateEntitiesFromPhysicsSystem>().After<UpdatePhysicSystem>();
    graph->AddSystem<UpdatePhysicSystem>();

    return true;
}

#if defined(RED_WINDOWS) && defined(RED_DEVBUILD)
static void LogToDebugger(const Logger::LogOoutputInfo& out)
{
    OutputDebugStringA(out.str.c_str());
    OutputDebugStringA("\n");
}
#endif

void Engine::SetupLogger()
{
    PROFILER_EVENT();

#ifdef RED_DEVBUILD
    // Always add standard output when debugging
    m_standarOutputFuncIndex = GetRedLogger()->AddOutput(Logger::LogToStandardOutputFun);
#endif  // RED_DEVBUILD

#if defined(RED_WINDOWS) && defined(RED_DEVBUILD)
    if (IsDebuggerPresent() != 0)
    {
        m_debugOutputFuncIndex = GetRedLogger()->AddOutput(LogToDebugger);
    }
#endif  // defined(RED_WINDOWS) && defined(RED_DEVBUILD)

    if (m_standarOutputFuncIndex == -1 && s_addStandardOutputLog)
    {
        m_standarOutputFuncIndex = GetRedLogger()->AddOutput(Logger::LogToStandardOutputFun);
    }

    SetLogLevel(s_logLevel);

    RED_LOG_INFO("Setup logger for output {}, debugger {}", m_standarOutputFuncIndex != -1,
                 m_debugOutputFuncIndex != -1);
}

void Engine::InitAllocator()
{
    auto workerCount = m_scheduler.GetWorkerCount();
    m_frameAllocator = (DoubleLinearAllocator*)red_malloc(workerCount * sizeof(DoubleLinearAllocator));

    for (int i = 0; i < workerCount; i++)
    {
        new(m_frameAllocator + i) DoubleLinearAllocator(RED_DEFAULT_FRAMEALLOCATOR_SIZE);
    }
}

bool Engine::Create()
{
    PROFILER_EVENT();

    InitSystemInfo();
    InitRandomEngine(42);

    CVarManager::LoadConfigFile(Path::Resource("Config.ini"));

    SetupLogger();

    m_scheduler.Init();

    InitAllocator();

    RedAssert(m_world == nullptr);
    m_world = red_new(World);

    m_renderer = red_new(Renderer);

    RegisterComponentTypes();
    RegisterSystems();

    m_world->Init();

    //// TODO Put it inside a resource loader system
    // auto* resourceHolder = worldEntity->AddComponent<ResourceHolderComponent>();
    // resourceHolder->RegisterResourceLoader(ResourceType::SPRITE, new SpriteResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::TEXTURE2D, new TextureResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::SOUND, new SoundResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::MATERIAL, new MaterialResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::GEOMETRY, new GeometryResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::FONT, new FontResourceLoader(m_world));
    // resourceHolder->RegisterResourceLoader(ResourceType::SHADER_PROGRAM, new ShaderProgramResourceLoader(m_world));

    return true;
}

bool Engine::Destroy()
{
    PROFILER_EVENT();

    /*auto* resourceHolder = m_world->GetWorldComponent<ResourceHolderComponent>();

    resourceHolder->RemoveAllLoaders();*/

    m_world->Finalize();

    red_delete(m_world);

    red_delete(m_renderer);

    red_free(m_frameAllocator);

    m_scheduler.Finalize();

    PROFILER_SHUTDOWN();

    GetRedLogger()->RemoveOutput(m_standarOutputFuncIndex);
    GetRedLogger()->RemoveOutput(m_debugOutputFuncIndex);

    return true;
}

StringView Engine::GetGameName() const
{
    return "RedEngine";
}

DoubleLinearAllocator& Engine::GetThreadFrameAllocator(int threadIndex)
{
    return m_frameAllocator[threadIndex];
}

ThreadScheduler& Engine::GetScheduler()
{
    return m_scheduler;
}

Renderer* Engine::GetRenderer()
{
    return m_renderer;
}
} // namespace red