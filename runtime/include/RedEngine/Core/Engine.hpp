#pragma once

#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Memory/LinearAllocator.hpp"
#include "RedEngine/Core/Memory/MemoryUtils.hpp"
#include "RedEngine/Thread/Thread.hpp"

#include "RedEngine/Core/Container/String.hpp"
#include <type_traits>

namespace red
{
class World;
class Renderer;

constexpr uint32 RED_DEFAULT_FRAMEALLOCATOR_SIZE = 50 * RED_MiB;

class Engine
{
public:
    template <typename EngineClass>
    friend EngineClass* CreateEngineFrom(int argc, char** argv);

    static Engine* GetInstance();

public:
    Engine();
    virtual ~Engine();

    void MainLoop();

    virtual bool Create();
    virtual bool Destroy();

    virtual StringView GetGameName() const;

    virtual bool RegisterComponentTypes();
    virtual bool RegisterSystems();

    virtual void SetupLogger();
    void InitAllocator();

    DoubleLinearAllocator& GetThreadFrameAllocator(int threadIndex);

    ThreadScheduler& GetScheduler();

    Renderer* GetRenderer();

protected:
    int m_argc;
    char** m_argv;

    World* m_world;

    Renderer* m_renderer;

    DoubleLinearAllocator* m_frameAllocator;  // dynamic array initialized when the threading system is init

    ThreadScheduler m_scheduler;

    Logger::OutputDelegate::FuncIndex m_standarOutputFuncIndex;
    Logger::OutputDelegate::FuncIndex m_debugOutputFuncIndex;

private:
    static Engine* s_engine;
};

}  // namespace red

#include "inl/Engine.inl"
