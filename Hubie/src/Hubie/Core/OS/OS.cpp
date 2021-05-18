#include "hbpch.h"
#include "OS.h"

#if defined(HB_PLATFORM_WINDOWS)
#include "Hubie/Platform/Windows/WindowsOS.h"
#elif defined(HB_PLATFORM_MACOS)

#elif defined(HB_PLATFORM_IOS)

#else
#include "Hubie/Platform/Unix/UnixOS.h"
#endif


namespace Hubie
{

	void OS::Create()
	{
		HB_ASSERT(!s_Instance, "OS already exists!");

#if defined(HB_PLATFORM_WINDOWS)
		s_Instance = new WindowsOS();
#elif defined(HB_PLATFORM_MACOS)

#elif defined(HB_PLATFORM_IOS)

#else
		s_Instance = new UnixOS();
#endif
	}

	void OS::Release()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	std::string OS::PowerStateToString(PowerState state)
	{
		switch (state)
		{
		case POWERSTATE_UNKNOWN:
			return std::string("Unknown");
			break;
		case POWERSTATE_NO_BATTERY:
			return std::string("No Battery");
			break;
		case POWERSTATE_CHARGED:
			return std::string("Charged");
			break;
		case POWERSTATE_CHARGING:
			return std::string("Charging");
			break;
		case POWERSTATE_ON_BATTERY:
			return std::string("On Battery");
			break;
		default:
			return std::string("Error");
			break;
		}
	}

	Hubie::OS* OS::s_Instance = nullptr;

}
