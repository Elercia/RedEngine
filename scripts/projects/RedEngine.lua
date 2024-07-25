newoption {
   trigger = "asan",
   description = "Build projects with address sanitizer"
}

include "Helpers.lua"

rootPath 				= os.getcwd() .. "/../../"
runtimePath 			= rootPath .. "runtime/"
externalPath 			= rootPath .. "externals/"
outputPath 				= rootPath .. "build/"
projectsFilesLocation 	= "./" .. os.target() .. "_" .. _ACTION
outputDirSementic 		= "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}-" .. _ACTION .. "/%{prj.name}"

print("Generating : " .. _ACTION .. " inside " .. "'" .. rootPath .. "'")

cppDialect 				= "C++20"
enableExceptions 		= false
libsToLink 				= {
							"GL3W",
							"SDL2",
							"Box2D",
							"fmt",
							"fmod",
							"fsbank",
							"fmodstudio",
							"optick",
							"STBI",
							"ImGui",
							"marl",
						}

availablePlatforms={}

if os.istarget("linux") then
	table.insert(availablePlatforms, "Linux64")
	table.insert(libsToLink, "dl")
elseif os.istarget("windows") then
	table.insert(availablePlatforms, "Win64")
	table.insert(libsToLink, "shell32")
	table.insert(libsToLink, "Advapi32")
end

function RedDefaultProjectOptions()
	language("C++")
	cppdialect(cppDialect)

	rtti("Off")
	if enableExceptions then
		exceptionhandling("On")
	else
		exceptionhandling("Off")
	end
	warnings("Extra")
	flags("NoPCH")
	staticruntime("Off")
	editandcontinue("Off")

	location(projectsFilesLocation)
	targetdir(outputPath .. "bin/" .. outputDirSementic)
	objdir (outputPath .. "obj/" .. outputDirSementic)

	filter { "options:asan", "configurations:Debug or ReleaseWithDebugInfo" }
		sanitize { "Address" }
		defines{"_DISABLE_VECTOR_ANNOTATION", "_DISABLE_STRING_ANNOTATION"}
	filter {}

	filter{ "platforms:Linux64" }
  		buildoptions {}
	filter {}

	filter { "toolset:msc" }
		linkoptions 
		{
			"/NODEFAULTLIB:library"
		}
	filter {}

	filter { "platforms:Win64" }
		systemversion "latest"

		defines
		{
			"RED_WINDOWS"
		}

		links
		{
			"SDL2main.lib"
		}
	filter {}

	filter { "platforms:Linux64" }
		defines
		{
			"RED_LINUX"
		}

		links
		{
			"pthread"
		}
	filter {}

	filter { "configurations:Debug" }
		defines 
		{
			"RED_DEBUG", 
			"RED_DEVBUILD", 
			"RED_BREAK_ON_ASSERT",
			"RED_USE_PROFILER" 
		}

		runtime "Debug"
		symbols "Full"
	filter {}

	filter { "configurations:ReleaseWithDebugInfo" }
		defines 
		{
			"RED_DEBUG_RELEASE",
			"RED_DEVBUILD",
			"RED_USE_PROFILER"
		}

		runtime "Release"
		optimize "on"
		symbols "Full"
	filter {}

	filter { "configurations:Release" }
		defines "RED_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "On"
	filter {}

	defines
	{
		"FMT_EXCEPTIONS=0",
	}
end

workspace "RedEngine"
	startproject "RedEngineLib"

	configurations
	{
		"Debug",
		"ReleaseWithDebugInfo",
		"Release"
	}

	platforms 
	{
		availablePlatforms
	}

	flags
	{
		"MultiProcessorCompile",
	}

	defines
	{
		"FMT_EXCEPTIONS=0",
	}

	filter { "platforms:Win64" }
		system "windows"
		architecture "x64"

		defines
		{
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter { "platforms:Linux64" }
		system "linux"
		architecture "x64"
	filter {}

	location(projectsFilesLocation)

	filter { "toolset:msc" }
  		buildoptions { "/Zc:hiddenFriend-" } -- Bug in MSVC : https://github.com/catchorg/Catch2/issues/2174
	filter {}

	filter { "toolset:clang" }
		buildoptions 
		{ 
			"-Wno-c++98-compat",
			"-Wno-c++98-compat-pedantic"
		} 
	filter {}


include "External.lua"
include "RedEngineLib.lua"
include "Templates.lua"
include "Tests.lua"
