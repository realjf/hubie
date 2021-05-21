IncludeDir = {}
IncludeDir["GLFW"] = "../Hubie/Deps/glfw/include/"
IncludeDir["Glad"] = "../Hubie/Deps/glad/include/"
IncludeDir["ImGui"] = "../Hubie/Deps/imgui/"
IncludeDir["ImGui2"] = "../Hubie/Deps/imgui/backends"
IncludeDir["spdlog"] = "../Hubie/Deps/spdlog/include"
IncludeDir["stb"] = "../Hubie/Deps/stb"
IncludeDir["SpirvCross"] = "../Hubie/Deps/SPIRV-Cross"
IncludeDir["Box2D"] = "../Hubie/Deps/box2d/include/"
IncludeDir["cereal"] = "../Hubie/Deps/cereal/include"
IncludeDir["entt"] = "../Hubie/Deps/entt/src"
IncludeDir["sol2"] = "../Hubie/Deps/sol2/include"
IncludeDir["Hubie"] = "../Hubie/src"

IncludeDir2 = {}
IncludeDir2["deps"] = "../Hubie/Deps"
IncludeDir2["openFBX"] = "../Hubie/Deps/OpenFBX/src"
IncludeDir2["tinygltf"] = "../Hubie/Deps/tinygltf"
IncludeDir2["tinyobjloader"] = "../Hubie/Deps/tinyobjloader"
IncludeDir2["meshoptimizer"] = "../Hubie/Deps/meshoptimizer/src"
IncludeDir2["volk"] = "../Hubie/Deps/volk"
IncludeDir2["glm"] = "../Hubie/Deps/glm"


project "Sandbox"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    files
    {
        "**.h",
        "**.cpp",
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "../Hubie/src",
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

    sysincludedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
        "%{IncludeDir.ImGui2}",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.Box2D}",
        "%{IncludeDir.SpirvCross}",
        "%{IncludeDir.cereal}",
        "%{IncludeDir.Hubie}"
	}

    links
    {
        "Hubie",
        "box2d",
        "imgui",
        "SpirvCross",
        "spdlog",
        "meshoptimizer"
    }

    defines
    {
        "SPDLOG_COMPILED_LIB"
    }

    filter "system:windows"
        systemversion "latest"

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
            "HB_PLATFORM_UNIX"
        }

        buildoptions
		{
			"-fpermissive",
			"-Wattributes",
			"-fPIC",
			"-Wignored-attributes",
			"-Wno-psabi"
		}

        links
		{
			"glfw",
		}

		links { "X11", "pthread", "dl", "atomic", "stdc++fs"}

    filter "configurations:Debug"
        defines { "HB_DEBUG" }
        symbols "On"
        runtime "Debug"
        optimize "Off"

    filter "configurations:Release"
        defines { "HB_RELEASE" }
        symbols "On"
        runtime "Release"
        optimize "Speed"
    
    filter "configurations:Production"
        defines { "HB_PRODUCTION" }
        symbols "Off"
        runtime "Release"
        optimize "Full"