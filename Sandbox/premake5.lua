IncludeDir = {}


project "Sandbox"
    kind "WindowedApp"
    language "C++"

    files
    {
        "**.h",
        "**.cpp",
    }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        optimize "Off"

    filter "configurations:Release"
        symbols "On"
        runtime "Release"
        optimize "Speed"
    
    filter "configurations:Production"
        symbols "Off"
        runtime "Release"
        optimize "Full"