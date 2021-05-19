#include "hbpch.h"
#include "CoreSystem.h"

#include "Version.h"

namespace Hubie
{

	namespace Internal
	{

		bool CoreSystem::Init(int argc /*= 0*/, char** argv /*= nullptr*/)
		{
			Debug::Log::Init();

			HB_INFO("Hubie Engine - Version {0}.{1}.{2}", HubieVersion.major, HubieVersion.minor, HubieVersion.patch);

			return true;
		}

		void CoreSystem::Shutdown()
		{
			HB_INFO("Shutting down System");

		}

	}
}
