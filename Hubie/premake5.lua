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
        "box2d",
        "imgui",
        "SpirvCross",
        "spdlog",
        "meshoptimizer"
    }

    defines
    {
        "HB_ENGINE",
        "HB_DYNAMIC",
        "SPDLOG_COMPILED_LIB"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"

        links
        {
            "glfw"
        }

        defines
        {
            "HB_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "system:linux"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "HB_PLATFORM_LINUX",
            "HB_PLATFORM_UNIX",
        }

        buildoptions
		{
			"-fpermissive",
			"-fPIC",
			"-Wignored-attributes",
			"-Wno-psabi"
		}

        links
        {
            "glfw",
            "X11",
            "pthread"
        }

        pchheader "../Hubie/src/hbpch.h"
        pchsource "../Hubie/src/hbpch.cpp"

    filter "configurations:Debug"
        defines { "HB_DEBUG" }
		symbols "On"
		runtime "Debug"
		optimize "Off"

	filter "configurations:Release"
        defines { "HB_RELEASE" }
		optimize "Speed"
		symbols "On"
		runtime "Release"

	filter "configurations:Production"
        defines { "HB_PRODUCTION" }
		symbols "Off"
		optimize "Full"
		runtime "Release"