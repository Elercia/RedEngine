group "RedEngine"

function RedEngineLib( name )
	project(name)
		kind("StaticLib")
		language("C++")
		cppdialect(cppDialect)

		RedDefaultProjectOptions()
		
		includedirs
		{
			runtimePath .. "/include/",
			runtimePath .. "/src/",
			ExternalIncludeDirs,
		}

		links
		{
			libsToLink
		}

		libdirs
		{
			ExternalLibDirs
		}
end

RedEngineLib("RedEngineLib")
	files
	{
		runtimePath .. "/runtime_compile.cpp",
	}

-- Project used to reference all the files of the engine projet so IDE are not lost
RedEngineLib("RedEngineLib_AllFiles")
	files
	{
		runtimePath .. "/include/**.hpp",
		runtimePath .. "/src/**.cpp",
	}

	removefiles
	{
		runtimePath .. "/runtime_compile.cpp",
	}

