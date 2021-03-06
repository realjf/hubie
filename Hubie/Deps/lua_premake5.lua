project 'lua'
	kind 'StaticLib'
	systemversion "latest"
	staticruntime "On"

	defines 
	{
		"LUA_COMPAT_MATHLIB=1"
	}

	local lua_src  = 'lua/src/'

	filter "configurations:dev or debug"
		defines "LUA_USE_APICHECK=1"
	filter {}

	files
	{
		lua_src .. '*.h',
		lua_src .. '*.c'
	}

	removefiles 
	{
		lua_src .. 'luac.c',
		lua_src .. 'lua.c'
	}

	filter "system:linux"
		buildoptions
    	{
    	  "-fPIC"
		}

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"

	filter "configurations:Production"
		optimize "On"
