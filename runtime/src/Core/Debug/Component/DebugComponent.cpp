#include "RedEngine/Core/Debug/Component/DebugComponent.hpp"

#include "RedEngine/Core/CoreModule.hpp"

#include "RedEngine/Core/Debug/DebugDraw/PhysicsDebugDraw.hpp"
#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Resources/ResourceHolderComponent.hpp"

namespace red
{
DebugComponent::DebugComponent() : m_physicsDebugDrawer(nullptr)
{
}

int DebugComponent::AddDebugDrawer(const char* name, DebugMenuDrawerFunc&& callback)
{
    static int debugDrawerIndex = 0;
    m_drawers.push_back({debugDrawerIndex++, name, std::move(callback)});

    return debugDrawerIndex - 1;
}

void DebugComponent::RemoveDebugDrawer(int id)
{
    auto index = m_drawers.FindIf([=](const DebugDrawer& e) { return e.id == id; });
    if (index != m_drawers.npos)
    {
        m_drawers.erase(m_drawers.begin() + index);
    }
}

const Array<DebugLinePoint>& DebugComponent::GetDebugLines() const
{
    return m_debugLines;
}

void DebugComponent::ClearDebug()
{
    m_debugLines.clear();
}

void DebugComponent::ClearLogs()
{
    m_logs.clear();
}

const Array<Logger::LogOoutputInfo>& DebugComponent::GetLogBuffer() const
{
    return m_logs;
}

void DebugComponent::HandleCommand(const String& str)
{
    Logger::LogOoutputInfo info;
    info.str = str;
    info.level = LogLevel::LEVEL_CUSTOM;
    m_logs.push_back(info);
}

void DebugComponent::AddLog(const Logger::LogOoutputInfo& str)
{
    m_logs.push_back(str);
    if (m_logs.size() > 1000)
        m_logs.erase(m_logs.begin(), m_logs.begin() + 100);
}

Array<Entity*>& DebugComponent::GetFilteredEntities()
{
    return m_filteredEntities;
}
}  // namespace red
