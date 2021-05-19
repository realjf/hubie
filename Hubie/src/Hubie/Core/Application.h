#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Scene/SceneManager.h"

namespace Hubie
{
	class HB_EXPORT Application
	{
	public:
		Application();
		virtual ~Application();
	};

	Application* CreateApplication();
}


