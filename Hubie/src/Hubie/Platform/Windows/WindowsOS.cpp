#include "hbpch.h"
#include "WindowsOS.h"

#include "Hubie/Core/Application.h"


#include <Windows.h>

extern Hubie::Application* Hubie::CreateApplication();

namespace Hubie
{
	void WindowsOS::Init()
	{
		
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

