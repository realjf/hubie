IncludeDir = {}


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

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HB_PLATFORM_WINDOWS"
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