#include "hbpch.h"
#include "WindowsOS.h"


#include "WindowsWindow.h"
#include "Hubie/Core/Application.h"



extern Hubie::Application* Hubie::CreateApplication();

namespace Hubie
{
	void WindowsOS::Init()
	{
#ifdef HB_USE_GLFW_WINDOWS
		GLFWWindow::MakeDefault();
#else
		WindowsWindow::MakeDefault();
#endif
	}

	void WindowsOS::Run()
	{
		HB_INFO("--------------------");
		HB_INFO(" System Information ");
		HB_INFO("--------------------");

		auto& app = Hubie::Application::Get();
		app.Init();
		app.Run();
		app.Release();
	}

}

