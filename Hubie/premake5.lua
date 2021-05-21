IncludeDir = {}
IncludeDir["GLFW"] = "./Deps/glfw/include/"
IncludeDir["Glad"] = "./Deps/glad/include/"
IncludeDir["ImGui"] = "./Deps/imgui/"
IncludeDir["ImGui2"] = "./Deps/imgui/backends"
IncludeDir["stb"] = "./Deps/stb/"
IncludeDir["Box2D"] = "./Deps/box2d/include"
IncludeDir["SpirvCross"] = "./Deps/SPIRV-Cross"
IncludeDir["spdlog"] = "./Deps/spdlog/include"
IncludeDir["cereal"] = "./Deps/cereal/include"
IncludeDir["entt"] = "./Deps/entt/src"
IncludeDir["sol2"] = "./Deps/sol2/include"
IncludeDir["Hubie"] = "src"

IncludeDir2 = {}
IncludeDir2["deps"] = "./Deps"
IncludeDir2["openFBX"] = "./Deps/OpenFBX/src"
IncludeDir2["tinygltf"] = "./Deps/tinygltf"
IncludeDir2["tinyobjloader"] = "./Deps/tinyobjloader"
IncludeDir2["meshoptimizer"] = "./Deps/meshoptimizer/src"
IncludeDir2["volk"] = "./Deps/volk"
IncludeDir2["glm"] = "./Deps/glm"
IncludeDir2["lua"] = "./Deps/lua/src"
IncludeDir2["freetype"] = "./Deps/freetype/include"


project "Hubie"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
	staticruntime "on"

    pchheader "hbpch.h"
    pchsource "src/hbpch.cpp"

    files
    {
        "src/**.h",
        "src/**.c",
        "src/**.cpp"
    }

    removefiles
	{
		"src/Hubie/Platform/**"
	}

    includedirs {
        "src/",
        "%{IncludeDir.entt}",
        "%{IncludeDir.sol2}",
        "%{IncludeDir.stb}",
        "%{IncludeDir2.openFBX}",
        "%{IncludeDir2.deps}",
        "%{IncludeDir2.volk}",
        "%{IncludeDir2.tinygltf}",
        "%{IncludeDir2.tinyobjloader}",
        "%{IncludeDir2.meshoptimizer}",
        "%{IncludeDir2.glm}",
        "%{IncludeDir2.freetype}",
        "%{IncludeDir2.lua}",
    }

    sysincludedirs {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui2}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.SpirvCross}",
        "%{IncludeDir.cereal}",
        "%{IncludeDir.Hubie}"
    }

    links
    {
        "lua",
        "box2d",
        "imgui",
        "freetype",
        "SpirvCross",
        "spdlog",
        "meshoptimizer"
    }

    defines
    {
        "HB_ENGINE",
        "HB_DYNAMIC",
        "SPDLOG_COMPILED_LIB",
        "FREEIMAGE_LIB",
        "HB_ROOT_DIR="  .. root_dir,
        "IMGUI_USER_CONFIG=\"src/Hubie/ImGui/ImConfig.h\"",
    }

    filter 'files:src/Hubie/Utilities/ExternalBuild.cpp'
        buildoptions
        {
	        "-w"
        }

	filter 'architecture:x86_64'
		defines { "HB_SSE" ,"USE_VMA_ALLOCATOR"}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"
        disablewarnings { 4307 }
		characterset ("MBCS")

        links
        {
            "glfw",
            "Dbghelp"
        }

        defines
        {
            "HB_PLATFORM_WINDOWS",
            "HB_RENDER_API_OPENGL",
			"HB_RENDER_API_VULKAN",
			"VK_USE_PLATFORM_WIN32_KHR",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_EXTENDED_ALIGNED_STORAGE",
			"_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING",
			"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
			"HB_IMGUI",
			"HB_OPENAL",
			"HB_VOLK",
			"HB_USE_GLFW_WINDOWS"
        }

        files
		{
			"src/Hubie/Platform/Windows/*.h",
			"src/Hubie/Platform/Windows/*.cpp",

			"src/Hubie/Platform/GLFW/*.h",
			"src/Hubie/Platform/GLFW/*.cpp",

			"src/Hubie/Platform/OpenAL/*.h",
			"src/Hubie/Platform/OpenAL/*.cpp",

			"src/Hubie/Platform/OpenGL/*.h",
			"src/Hubie/Platform/OpenGL/*.cpp",

			"src/Hubie/Platform/Vulkan/*.h",
			"src/Hubie/Platform/Vulkan/*.cpp",

			"Deps/glad/src/glad_wgl.c"
		}

        buildoptions
		{
			"/MP", "/bigobj"
		}

		filter 'files:Deps/**.cpp'
			flags  { 'NoPCH' }
		filter 'files:Deps/**.c'
			flags  { 'NoPCH' }

    filter "system:linux"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "HB_PLATFORM_LINUX",
            "HB_PLATFORM_UNIX",
            "HB_RENDER_API_OPENGL",
			"HB_RENDER_API_VULKAN",
			"VK_USE_PLATFORM_XCB_KHR",
			"HB_IMGUI",
			"HB_VOLK"
        }

        files
		{
			"src/Hubie/Platform/Unix/*.h",
			"src/Hubie/Platform/Unix/*.cpp",

			"src/Hubie/Platform/GLFW/*.h",
			"src/Hubie/Platform/GLFW/*.cpp",

			"src/Hubie/Platform/OpenAL/*.h",
			"src/Hubie/Platform/OpenAL/*.cpp",

			"src/Hubie/Platform/OpenGL/*.h",
			"src/Hubie/Platform/OpenGL/*.cpp",

			"src/Hubie/Platform/Vulkan/*.h",
			"src/Hubie/Platform/Vulkan/*.cpp",
		}

        buildoptions
		{
			"-fpermissive",
			"-fPIC",
			"-Wignored-attributes",
			"-Wno-psabi"
		}

        -- linkoptions
		-- {
		-- 	"Deps/OpenAL/libs/linux/libopenal.so"
		-- }

        linkoptions{ "-Wl,-rpath=\\$$ORIGIN" }

        links
        {
            "glfw",
            "X11",
            "pthread"
        }

        pchheader "../Hubie/src/hbpch.h"
        pchsource "../Hubie/src/hbpch.cpp"

        filter 'files:Deps/**.cpp'
			flags  { 'NoPCH' }
		filter 'files:Deps/**.c'
			flags  { 'NoPCH' }
		filter 'files:src/Hubie/**.c'
			flags  { 'NoPCH' }

		filter {'system:linux', 'architecture:x86_64'}
			buildoptions
			{
				"-msse4.1",
			}

    filter "configurations:Debug"
        defines { "HB_DEBUG", "_DEBUG","TRACY_ENABLE","HB_PROFILE" }
		symbols "On"
		runtime "Debug"
		optimize "Off"

	filter "configurations:Release"
        defines { "HB_RELEASE" ,"TRACY_ENABLE","HB_PROFILE"}
		optimize "Speed"
		symbols "On"
		runtime "Release"

	filter "configurations:Production"
        defines { "HB_PRODUCTION" }
		symbols "Off"
		optimize "Full"
		runtime "Release"