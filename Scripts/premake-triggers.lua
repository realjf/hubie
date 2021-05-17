newoption
{
    trigger = "renderer",
    value = "API",
    description = "Choose a renderer",
    allowed = 
    {
        { "opengl", "OpenGL (macOS, linux, Windows)" },
        { "vulkan", "Vulkan (Windows, linux, iOS, macOS)" }
    }
}

newoption
{
    trigger = "arch",
    value = "arch",
    description = "Choose architecture",
    allowed = 
    {
        {"x86", "x86"},
        {"x64", "x64"},
        {"arm", "arm"},
        {"arm64", "arm64"}
    }
}

newaction
{
	trigger     = "clean",
	description = "clean the software",
	execute     = function ()
		print("clean the build...")
		os.rmdir("./build")
		os.rmdir("./bin")
		os.rmdir("./bin-int")
		print("done.")
	end
}