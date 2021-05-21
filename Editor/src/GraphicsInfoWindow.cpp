#include "GraphicsInfoWindow.h"
#include <Hubie/Graphics/API/GraphicsContext.h>

#include <imgui/imgui.h>

namespace Hubie
{
	GraphicsInfoWindow::GraphicsInfoWindow()
	{
		m_Name = "GraphicsInfo";
		m_SimpleName = "GraphicsInfo";
	}

	void GraphicsInfoWindow::OnImGui()
	{
		auto flags = ImGuiWindowFlags_NoCollapse;
		ImGui::Begin("GraphicsInfo", &m_Active, flags);
		{
			Graphics::GraphicsContext::GetContext()->OnImGui();
		}
		ImGui::End();
	}
}
