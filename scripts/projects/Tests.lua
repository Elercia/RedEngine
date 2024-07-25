group("Tests")

project ("RedEngineTest")
	kind "ConsoleApp"
	language("C++")
	
	RedDefaultProjectOptions()

	local testPath = rootPath .. "tests/"
	files {
		testPath .. "src/**.cpp",
		testPath .. "include/**.hpp",
		externalDirectoryPath .. "/Catch2/src/*.cpp"
	}

	includedirs
	{
		runtimePath .. "include/",
		testPath .. "include/",
		ExternalIncludeDirs,
	}

	libdirs
	{
		ExternalLibDirs
	}

	links
	{
		"RedEngineLib",
		"glm",
		libsToLink
	}

	exceptionhandling("On")

	postbuildcommands { "{COPYFILE} \"%{cfg.buildtarget.abspath}\" \"" .. rootPath .. "tests\"" }
