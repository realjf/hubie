project "spdlog"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    defines
    {
        "SPDLOG_COMPILED_LIB"
    }

    files {
        "spdlog/src/**.cpp",
        "spdlog/include/**.h"
    }

    sysincludedirs
    {
        "spdlog/include"
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
		defines "NDEBUG"
		optimize "On"

	filter "configurations:Production"
		optimize "On" 