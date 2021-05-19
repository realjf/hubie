
#if defined(HB_PLATFORM_WINDOWS)

#include "CoreSystem.h"
#include "Hubie/Platform/Windows/WindowsOS.h"

#pragma comment(linker, "/subsystem:windows")

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

extern Hubie::Application* Hubie::CreateApplication();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!Hubie::Internal::CoreSystem::Init(0, nullptr))
		return 0;

	HB_INFO("hello world");

	auto windowsOS = new Hubie::WindowsOS();
	Hubie::OS::SetInstance(windowsOS);

	windowsOS->Init();

	Hubie::CreateApplication();

	windowsOS->Run();
	delete windowsOS;

	Hubie::Internal::CoreSystem::Shutdown();
}



#elif defined(HB_PLATFORM_LINUX)

#include "CoreSystem.h"
#include "Hubie/Platform/Unix/UnixOS.h"

extern Hubie::Application* Hubie::CreateApplication();

int main(int argc, char** argv)
{
	if (!Hubie::Internal::CoreSystem::Init(0, nullptr))
		return 0;

	auto unixOS = new Hubie::UnixOS();
	Hubie::OS::SetInstance(unixOS);
	unixOS->Init();

	Hubie::CreateApplication();

	unixOS->Run();
	delete unixOS;

	Hubie::Internal::CoreSystem::Shutdown();
}



#elif defined(HB_PLATFORM_MACOS)


int main(int argc, char** argv)
{

}


#elif defined(HB_PLATFORM_IOS)



#endif
