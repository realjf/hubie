#pragma once

#if defined(HB_PLATFORM_WINDOWS)

#include "CoreSystem.h"
#include "Hubie/Platform/Windows/WindowsOS.h"

#pragma comment(linker, "/subsystem:windows")

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

int WinMain()
{

}



#elif defined(HB_PLATFORM_LINUX)

#include "Core/CoreSystem.h"
#include "Platform/Unix/UnixOS.h"

int main(int argc, char** argv)
{
	Hubie::Log::Init();
	
}



#elif defined(HB_PLATFORM_MACOS)


int main(int argc, char** argv)
{

}


#elif defined(HB_PLATFORM_IOS)



#endif
