#pragma once

#include "EditorWindow.h"

namespace Hubie
{
	class ApplicationInfoWindow : public EditorWindow
	{
	public:
		ApplicationInfoWindow();
		~ApplicationInfoWindow() = default;

		void OnImGui() override;
	};
}