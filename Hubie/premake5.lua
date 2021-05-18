IncludeDir = {}
IncludeDir["GLFW"] = "./Deps/glfw/include/"
IncludeDir["Glad"] = "./Deps/glad/include/"
IncludeDir["ImGui"] = "./Deps/imgui/"
IncludeDir["spdlog"] = "./Deps/spdlog/include"
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
        "src/"
    }

    sysincludedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.Hubie}"
    }

    links
    {
        "imgui",
        "spdlog"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "on"
        systemversion "latest"

        links
        {
            "opengl32.lib"
        }

        defines
        {
            "HB_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE",
            "HB_BUILD_DLL"
        }

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