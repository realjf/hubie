#pragma once

#include "EditorWindow.h"

namespace Hubie
{
	class GraphicsInfoWindow : public EditorWindow
	{
	public:
		GraphicsInfoWindow();
		~GraphicsInfoWindow() = default;

		void OnImGui() override;
	};
}