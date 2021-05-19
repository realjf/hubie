#pragma once

#include "Hubie/Core/Core.h"


namespace Hubie
{
	namespace Internal
	{
		class HB_EXPORT CoreSystem
		{
		public:
			static bool Init(int argc = 0, char** argv = nullptr);
			static void Shutdown();
		};
	}
}

