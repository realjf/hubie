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
IncludeDir2["lua"] = "../Hubie/Deps/lua/src"
IncludeDir2["freetype"] = "../Hubie/Deps/freetype/include"
IncludeDir2["OpenAL"] = "../Hubie/Deps/OpenAL/include"

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
        "%{IncludeDir2.lua}",
        "%{IncludeDir2.freetype}",
        "%{IncludeDir2.OpenAL}",
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
        "lua",
        "freetype",
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

    filter { "files:Deps/**"}
		warnings "Off"

	filter 'architecture:x86_64'
		defines { "HB_SSE" ,"USE_VMA_ALLOCATOR"}

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"
		systemversion "latest"
		entrypoint "WinMainCRTStartup"

        defines
        {
            "HB_PLATFORM_WINDOWS",
            "HB_RENDER_API_OPENGL",
			"HB_RENDER_API_VULKAN",
			"VK_USE_PLATFORM_WIN32_KHR",
			"WIN32_LEAN_AND_MEAN",
			"_CRT_SECURE_NO_WARNINGS",
			"_DISABLE_EXTENDED_ALIGNED_STORAGE",
			"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING",
			"HB_ROOT_DIR="  .. root_dir,
			"HB_VOLK"
        }

        -- libdirs
		-- {
		-- 	"../Hubie/Deps/OpenAL/libs/Win32"
		-- }

        links
		{
			"glfw",
			"OpenGL32",
			"OpenAL32"
		}

		disablewarnings { 4307 }

    filter "system:linux"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "HB_PLATFORM_LINUX",
            "HB_PLATFORM_UNIX",
            "HB_RENDER_API_OPENGL",
			"HB_RENDER_API_VULKAN",
			"VK_USE_PLATFORM_XCB_KHR",
			"HB_IMGUI",
			"HB_ROOT_DIR="  .. root_dir,
			"HB_VOLK"
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

        linkoptions { "-L%{cfg.targetdir}", "-Wl,-rpath=\\$$ORIGIN" }

		filter {'system:linux', 'architecture:x86_64'}
			buildoptions
			{
				"-msse4.1",
			}

    filter "configurations:Debug"
        defines { "HB_DEBUG", "_DEBUG","TRACY_ENABLE","HB_PROFILE"}
        symbols "On"
        runtime "Debug"
        optimize "Off"

    filter "configurations:Release"
        defines { "HB_RELEASE","TRACY_ENABLE","HB_PROFILE", }
        symbols "On"
        runtime "Release"
        optimize "Speed"
    
    filter "configurations:Production"
        defines { "HB_PRODUCTION" }
        symbols "Off"
        runtime "Release"
        optimize "Full"