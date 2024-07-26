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

	-- GLM output some warnings
	disablewarnings{	"4828", 	-- The file contains a character starting at offset ... that is illegal in the current source character set 
						"5033", 	-- 'register' is no longer a supported storage class
						"4201", 	-- nonstandard extension used: nameless struct/union
					}

	postbuildcommands { "{COPYFILE} \"%{cfg.buildtarget.abspath}\" \"" .. rootPath .. "tests\"" }
