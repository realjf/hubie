#pragma once

#if defined(HB_PLATFORM_WINDOWS)

#include "CoreSystem.h"
#include "Hubie/Platform/Windows/WindowsOS.h"

#pragma comment(linker, "/subsystem:windows")

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

extern Hubie::Application* Hubie::CreateApplication();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	Hubie::Log::Init();
	HB_INFO("hello world");

	auto windowsOS = new Hubie::WindowsOS();
	Hubie::OS::SetInstance(windowsOS);

	windowsOS->Init();

	windowsOS->Run();
	delete windowsOS;

}



#elif defined(HB_PLATFORM_LINUX)

#include "CoreSystem.h"
#include "Hubie/Platform/Unix/UnixOS.h"

extern Hubie::Application* Hubie::CreateApplication();

int main(int argc, char** argv)
{
	Hubie::Log::Init();

	auto unixOS = new Hubie::UnixOS();
	Hubie::OS::SetInstance(unixOS);
	unixOS->Init();

	unixOS->Run();
	delete unixOS;
	
}



#elif defined(HB_PLATFORM_MACOS)


int main(int argc, char** argv)
{

}


#elif defined(HB_PLATFORM_IOS)



#endif
