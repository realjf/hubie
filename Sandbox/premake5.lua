IncludeDir = {}
IncludeDir["GLFW"] = "../Hubie/Deps/glfw/include/"
IncludeDir["Glad"] = "../Hubie/Deps/glad/include/"
IncludeDir["ImGui"] = "../Hubie/Deps/imgui/"
IncludeDir["spdlog"] = "../Hubie/Deps/spdlog/include"
IncludeDir["stb"] = "../Hubie/Deps/stb"
IncludeDir["Hubie"] = "../Hubie/src"

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
        "../Hubie/src"
    }

    sysincludedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.Hubie}"
	}

    links
    {
        "Hubie",
    }

    defines
    {
        "HB_ENGINE",
        "HB_DYNAMIC"
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