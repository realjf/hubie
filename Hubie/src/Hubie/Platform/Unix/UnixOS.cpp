#include "Precompiled.h"
#include "UnixOS.h"
#include "Hubie/Platform/GLFW/GLFWWindow.h"
#include "Hubie/Core/CoreSystem.h"
#include "Hubie/Core/OS/MemoryManager.h"
#include "Hubie/Core/Application.h"

extern Hubie::Application* Hubie::CreateApplication();

namespace Hubie
{
    void UnixOS::Run()
    {
        const std::string root = ROOT_DIR;
        VFS::Get()->Mount("Meshes", root + "/Assets/meshes");
        VFS::Get()->Mount("Textures", root + "/Assets/textures");
        VFS::Get()->Mount("Sounds", root + "/Assets/sounds");

        auto& app = Hubie::Application::Get();
        app.Init();
        app.Run();
        app.Release();
    }

    void UnixOS::Init()
    {
        GLFWWindow::MakeDefault();
    }

    SystemMemoryInfo MemoryManager::GetSystemInfo()
    {
        return SystemMemoryInfo();
    }
}
