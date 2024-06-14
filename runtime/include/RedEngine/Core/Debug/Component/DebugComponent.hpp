#pragma once

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Debug/DebugDraw/PhysicsDebugDraw.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Math/Vector.hpp"
#include "RedEngine/Rendering/Color.hpp"

#include <memory>

namespace red
{

struct DebugLinePoint
{
    Vector4 pos;
    Color color;
};

using DebugMenuDrawerFunc = void (*)(DebugComponent*);

struct DebugDrawer
{
    int id;
    String name;
    DebugMenuDrawerFunc callback;
};

class DebugComponent
{
    friend class DebugSystem;
    friend class RenderingSystem;

public:
    explicit DebugComponent();
    ~DebugComponent() = default;

    int AddDebugDrawer(const char* name, DebugMenuDrawerFunc&& callback);
    void RemoveDebugDrawer(int id);

    const Array<DebugLinePoint>& GetDebugLines() const;
    void ClearDebug();

    void ClearLogs();
    const Array<Logger::LogOoutputInfo>& GetLogBuffer() const;
    void HandleCommand(const String& str);
    void AddLog(const Logger::LogOoutputInfo& str);
    Array<Entity*>& GetFilteredEntities();

    Array<DebugLinePoint> m_debugLines;

    std::unique_ptr<PhysicsDebugDrawer> m_physicsDebugDrawer;
    Logger::OutputDelegate::FuncIndex m_outputFuncIndex;
    Array<Logger::LogOoutputInfo> m_logs;
    Array<DebugDrawer> m_drawers;
    Array<Entity*> m_filteredEntities;
};
RED_DECLARE_SINGLETON_COMPONENT(DebugComponent);

}  // namespace red
