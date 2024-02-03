workspace "HelloPremake"
	filename "HelloPremake"
   	configurations { "Debug", "Release" } -- default is Debug
	toolset "gcc"
	gccprefix ""
	language "C"
	staticruntime "on"
	-- cdialect "C99" -- 

	targetdir "build"
	os.mkdir("build")
	os.mkdir("obj")
	objdir "!obj\\%{cfg.buildcfg}"

	filter{"configurations:Debug"}
		runtime "Debug"
		kind "WindowedApp"
		os.mkdir("obj\\Debug")
        buildoptions{"-O0", "-D__SOTA_DEBUG__", "-DSDL_ASSERT_LEVEL=2 -I\\msys64\\mingw64\\include"}
        symbols "on"

	filter{"configurations:Release"}
		runtime "Release"
		kind "WindowedApp"
		os.mkdir("obj\\Release")
		-- optimize "on" -- what does on do -> O1?
		buildoptions{"-O2", "-D__SOTA_RELEASE__", "-DSDL_ASSERT_LEVEL=2 -I\\msys64\\mingw64\\include"}

	filter "kind:ConsoleApp"
		files{
			"src/*.c",
			"src/bars/*.c",
			"src/controller/*.c",
			"src/game/*.c",
			"src/map/*.c",
			"src/menu/*.c",
			"src/popup/*.c",
			"src/systems/*.c",
			"src/unit/*.c",
		}
		includedirs{
			".",
			"include",
			"include/bars",
			"include/controller",
			"include/game",
			"include/map",
			"include/menu",
			"include/popup",
			"include/systems",
			"include/unit",
			"names",
			"scenes",
			"second_party/**",
			"third_party/**",
		}
	filter {"kind:ConsoleApp", "system:Windows"}
		links { "mingw32", "SDL2main", "SDL2", "SDL2_mixer", "SDL2_image", "glew32", "opengl32","cJSON", "nmath", "physfs", "tinymt", "tnecs", "nstr", "parg"}
 	filter {"kind:ConsoleApp", "system:Linux"}
		links { "SDL2", "SDL2_image", "SDL2_mixer", "m", "GLEW", "cJSON", "nmath", "physfs", "tinymt", "tnecs", "nstr", "parg"}

project "cJSON"
	kind "StaticLib"
	files{"third_party/cJson/**.c"}
	includedirs{"third_party/cJson"}
	buildoptions{"-std=iso9899:1999"}

project "physfs"
	kind "StaticLib"
	files{"third_party/physfs/**.c"}
	includedirs{"third_party/physfs"}

project "tinymt"
	kind "StaticLib"
	files{"third_party/tinymt/**.c"}
	includedirs{ "third_party/tinymt"}
	buildoptions{"-std=iso9899:1999"}

project "nmath"
	kind "StaticLib"
	basedir("second_party/noursmath")
	files{"second_party/noursmath/**.c"}
	includedirs{ "second_party/noursmath"}
	buildoptions{"-std=iso9899:1999"}

project "tnecs"
	kind "StaticLib"
	basedir("second_party/tnecs")
	files{"second_party/tnecs/**.c"}
	includedirs{ "second_party/tnecs"}
	-- buildoptions{"-std=iso9899:1999"}

project "nstr"
	kind "StaticLib"
	basedir("second_party/nstr")
	files{"second_party/nstr/**.c"}
	includedirs{ "second_party/nstr"}
	buildoptions{"-std=iso9899:1999"}

project "parg"
	kind "StaticLib"
	basedir("second_party/parg")
	files{"second_party/parg/**.c"}
	includedirs{ "second_party/parg"}

project "sota"
	filename "sota"
	rules { "CustomRule" }
	kind "ConsoleApp"
 	filter {"kind:ConsoleApp", "system:Linux"}
		prebuildmessage("--- astyle formatting ---")
		prebuildcommands {'@command -v astyle && astyle --options=utils/style.txt --verbose --recursive "src/*" "include/*" "test/*" "names/*" || true'	}

project "test"
	filename "sota_test"
	kind "ConsoleApp"
	files{
		"test/**.c",
	}
	removefiles{"src/main.c"}

rule "CustomRule"
   fileextension ".c"
   buildmessage "Compiling '%(Filename)'"
   buildcommands 'gcc -c "%(FullPath)" -o "%(IntDir)/%(Filename).o"'
   buildoutputs "%(IntDir)/%(Filename).o"
