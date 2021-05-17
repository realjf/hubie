IncludeDir = {}
IncludeDir["GLFW"] = "Deps/glfw/include/"
IncludeDir["Glad"] = "Deps/glad/include/"
IncludeDir["ImGui"] = "Deps/imgui/"
IncludeDir["spdlog"] = "External/spdlog/include"

project "Hubie"
    kind "StaticLib"
    language "C++"


    files
    {
        "src/**.h",
        "src/**.c",
        "src/**.cpp"
    }

    sysincludedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.ImGui}"
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

        pchheader "hbpch.h"
        pchsource "src/hbpch.cpp"

    filter "configurations:Debug"
		symbols "On"
		runtime "Debug"
		optimize "Off"

	filter "configurations:Release"
		optimize "Speed"
		symbols "On"
		runtime "Release"

	filter "configurations:Production"
		symbols "Off"
		optimize "Full"
		runtime "Release"