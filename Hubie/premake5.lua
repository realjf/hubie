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
IncludeDir["openfbx"] = "./Deps/OpenFBX/src"
IncludeDir["tinygltf"] = "./Deps/tinygltf"
IncludeDir["tinyobjloader"] = "./Deps/tinyobjloader"
IncludeDir["meshoptimizer"] = "./Deps/meshoptimizer/src"
IncludeDir["volk"] = "./Deps/volk"
IncludeDir["glm"] = "./Deps/glm"
IncludeDir["Hubie"] = "src"

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

    includedirs
    {
        "src/",
        "%{IncludeDir.entt}",
        "%{IncludeDir.sol2}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.openfbx}",
        "%{IncludeDir.tinygltf}",
        "%{IncludeDir.tinyobjloader}",
        "%{IncludeDir.meshoptimizer}",
        "%{IncludeDir.volk}",
    }

    sysincludedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
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