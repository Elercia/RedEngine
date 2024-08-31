// Headers
#include "RedEngine/Audio/AudioModule.hpp"
#include "RedEngine/Audio/AudioEvent.hpp"
#include "RedEngine/Audio/AudioUtils.hpp"
#include "RedEngine/Audio/Component/AudioSubSystem.hpp"
#include "RedEngine/Audio/Component/AudioListener.hpp"
#include "RedEngine/Audio/Component/AudioSource.hpp"
#include "RedEngine/Audio/Resource/SoundResource.hpp"
#include "RedEngine/Audio/Resource/SoundResourceLoader.hpp"
#include "RedEngine/Audio/System/AudioSystem.hpp"

#include "RedEngine/Core/CoreModule.hpp"
#include "RedEngine/Core/Configuration/CVar.hpp"
#include "RedEngine/Core/Configuration/CVarManager.hpp"
#include "RedEngine/Core/Configuration/CVarUtils.hpp"
#include "RedEngine/Core/Configuration/IniReader.hpp"
#include "RedEngine/Core/Configuration/UserInputHelper.hpp"

#include "RedEngine/Core/Container/Array.hpp"
#include "RedEngine/Core/Container/Map.hpp"
#include "RedEngine/Core/Container/String.hpp"

#include "RedEngine/Core/Debug/Component/DebugComponent.hpp"
#include "RedEngine/Core/Debug/DebugDraw/PhysicsDebugDraw.hpp"
#include "RedEngine/Core/Debug/DebugMacros.hpp"
#include "RedEngine/Core/Debug/Logger/Logger.hpp"
#include "RedEngine/Core/Debug/Profiler.hpp"
#include "RedEngine/Core/Debug/System/DebugSystem.hpp"

#include "RedEngine/Core/Engine.hpp"

#include "RedEngine/Core/Event/Component/EventsComponent.hpp"
#include "RedEngine/Core/Event/Delegate.hpp"
#include "RedEngine/Core/Event/Signal.hpp"
#include "RedEngine/Core/Event/System/EventSystem.hpp"

#include "RedEngine/Core/Macros.hpp"

#include "RedEngine/Core/Memory/LinearAllocator.hpp"
#include "RedEngine/Core/Memory/Macros.hpp"
#include "RedEngine/Core/Memory/MemoryProfiler.hpp"
#include "RedEngine/Core/Memory/MemoryUtils.hpp"
#include "RedEngine/Core/Memory/PoolAllocator.hpp"

#include "RedEngine/Core/SerializationFunctions.hpp"

#include "RedEngine/Core/Time/FrameCounter.hpp"
#include "RedEngine/Core/Time/Time.hpp"

#include "RedEngine/Entity/Entity.hpp"
#include "RedEngine/Entity/SystemQuery.hpp"
#include "RedEngine/Entity/System.hpp"
#include "RedEngine/Entity/Transform.hpp"
#include "RedEngine/Entity/ComponentRegistry.hpp"
#include "RedEngine/Entity/ComponentManager.hpp"
#include "RedEngine/Entity/World.hpp"
#include "RedEngine/Entity/SystemExecutionGraph.hpp"

#include "RedEngine/Filesystem/FileSystemModule.hpp"
#include "RedEngine/Filesystem/File.hpp"
#include "RedEngine/Filesystem/Path.hpp"

#include "RedEngine/Input/InputModule.hpp"
#include "RedEngine/Input/Component/UserInput.hpp"
#include "RedEngine/Input/InputDefinition.hpp"
#include "RedEngine/Input/InputDefinitionTranslationUnit.hpp"
#include "RedEngine/Input/System/UserInputSystem.hpp"
#include "RedEngine/Input/UserInputDefinition.hpp"

#include "RedEngine/Math/MathModule.hpp"
#include "RedEngine/Math/AABB.hpp"
#include "RedEngine/Math/Hash.hpp"
#include "RedEngine/Math/Math.hpp"
#include "RedEngine/Math/Matrix.hpp"
#include "RedEngine/Math/MatrixFunctions.hpp"
#include "RedEngine/Math/Vector.hpp"

#include "RedEngine/Physics/PhysicsModule.hpp"
#include "RedEngine/Physics/Components/PhysicBody.hpp"
#include "RedEngine/Physics/ContactInfo.hpp"
#include "RedEngine/Physics/Components/PhysicsWorld.hpp"
#include "RedEngine/Physics/System/PhysicsSystem.hpp"

#include "RedEngine/Rendering/Color.hpp"
#include "RedEngine/Rendering/Renderer.hpp"

#include "RedEngine/Resources/ResourceModule.hpp"
#include "RedEngine/Resources/Resource.hpp"
#include "RedEngine/Resources/ResourceHolderComponent.hpp"
#include "RedEngine/Resources/ResourceLoader.hpp"

#include "RedEngine/Thread/ExecutionGraph.hpp"
#include "RedEngine/Thread/Thread.hpp"

#include "RedEngine/Utils/UtilityModule.hpp"
#include "RedEngine/Utils/BitfieldUtils.hpp"
#include "RedEngine/Utils/FileUtils.hpp"
#include "RedEngine/Utils/Random.hpp"
#include "RedEngine/Utils/StringUtils.hpp"
#include "RedEngine/Utils/SystemInfo.hpp"
#include "RedEngine/Utils/Types.hpp"
#include "RedEngine/Utils/TypesInfo.hpp"
#include "RedEngine/Utils/Uncopyable.hpp"

// Sources
#include "Audio/AudioUtils.cpp"
#include "Audio/Component/AudioListener.cpp"
#include "Audio/Component/AudioSource.cpp"
#include "Audio/Resource/SoundResource.cpp"
#include "Audio/Resource/SoundResourceLoader.cpp"
#include "Audio/System/AudioSystem.cpp"

#include "Core/Configuration/CVar.cpp"
#include "Core/Configuration/CVarManager.cpp"
#include "Core/Configuration/CVarUtils.cpp"
#include "Core/Configuration/IniReader.cpp"

#include "Core/Debug/Component/DebugComponent.cpp"
#include "Core/Debug/DebugDraw/PhysicsDebugDraw.cpp"
#include "Core/Debug/DebugMacros.cpp"
#include "Core/Debug/Logger/Logger.cpp"
#include "Core/Debug/System/DebugSystem.cpp"

#include "Core/Engine.cpp"

#include "Core/Event/Component/EventsComponent.cpp"
#include "Core/Event/Delegate.cpp"
#include "Core/Event/System/EventSystem.cpp"

#include "Core/Memory/LinearAllocator.cpp"
#include "Core/Memory/MemoryProfiler.cpp"
#include "Core/Memory/MemoryUtils.cpp"
#include "Core/Memory/PoolAllocator.cpp"

#include "Core/SerializationFunctions.cpp"

#include "Core/Time/FrameCounter.cpp"
#include "Core/Time/Time.cpp"

#include "Entity/ComponentRegistry.cpp"
#include "Entity/ComponentManager.cpp"
#include "Entity/World.cpp"
#include "Entity/SystemExecutionGraph.cpp"

#include "Filesystem/File.cpp"
#include "Filesystem/Path.cpp"

#include "Input/Component/UserInput.cpp"
#include "Input/InputDefinition.cpp"
#include "Input/System/UserInputSystem.cpp"
#include "Input/UserInputHelper.cpp"

#include "Math/Math.cpp"
#include "Math/SerializationFunction.cpp"
#include "Math/Vector.cpp"

#include "Physics/Components/PhysicBody.cpp"
#include "Physics/ContactInfo.cpp"
#include "Physics/Components/PhysicsWorld.cpp"
#include "Physics/System/PhysicSystem.cpp"

#include "Rendering/Renderer.cpp"

#include "Resources/Resource.cpp"
#include "Resources/ResourceHolderComponent.cpp"
#include "Resources/ResourceLoader.cpp"

#include "Thread/ExecutionGraph.cpp"
#include "Thread/Thread.cpp"

#include "Utils/FileUtils.cpp"
#include "Utils/Random.cpp"
#include "Utils/StringUtils.cpp"
#include "Utils/SystemInfo.cpp"
#include "Utils/Types.cpp"
#include "Utils/TypesInfo.cpp"
