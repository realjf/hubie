#include "Precompiled.h"
#include "WindowsOS.h"
#include "WindowsPower.h"
#include "WindowsWindow.h"
#include "Hubie/Core/CoreSystem.h"
#include "Hubie/Core/OS/MemoryManager.h"
#include "Hubie/Core/Application.h"

#ifdef HB_USE_GLFW_WINDOWS
#include "Hubie/Platform/GLFW/GLFWWindow.h"
#endif

#include <Windows.h>

extern Hubie::Application* Hubie::CreateApplication();

namespace Hubie
{
    void WindowsOS::Run()
    {
        auto power = WindowsPower();
        auto percentage = power.GetPowerPercentageLeft();
        auto secondsLeft = power.GetPowerSecondsLeft();
        auto state = power.GetPowerState();

        HB_INFO("--------------------");
        HB_INFO(" System Information ");
        HB_INFO("--------------------");

        if(state != PowerState::POWERSTATE_NO_BATTERY)
            HB_INFO("Battery Info - Percentage : {0} , Time Left {1}s , State : {2}", percentage, secondsLeft, PowerStateToString(state));
        else
            HB_INFO("Power - Outlet");

        auto systemInfo = MemoryManager::Get()->GetSystemInfo();
        systemInfo.Log();

        const std::string root = ROOT_DIR;
        VFS::Get()->Mount("Meshes", root + "/Assets/meshes");
        VFS::Get()->Mount("Textures", root + "/Assets/textures");
        VFS::Get()->Mount("Sounds", root + "/Assets/sounds");

        auto& app = Hubie::Application::Get();
        app.Init();
        app.Run();
        app.Release();
    }

    void WindowsOS::Init()
    {
#ifdef HB_USE_GLFW_WINDOWS
        GLFWWindow::MakeDefault();
#else
        WindowsWindow::MakeDefault();
#endif
    }

    SystemMemoryInfo MemoryManager::GetSystemInfo()
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&status);

        SystemMemoryInfo result = {
            (int64_t)status.ullAvailPhys,
            (int64_t)status.ullTotalPhys,

            (int64_t)status.ullAvailVirtual,
            (int64_t)status.ullTotalVirtual
        };
        return result;
    }
}
