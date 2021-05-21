#include "HierarchyWindow.h"
#include "Editor.h"
#include "ApplicationInfoWindow.h"

#include <Hubie/Graphics/API/GraphicsContext.h>
#include <Hubie/Core/Application.h>
#include <Hubie/Scene/SceneManager.h>
#include <Hubie/Core/Engine.h>
#include <Hubie/Graphics/Renderers/RenderGraph.h>
#include <Hubie/Graphics/GBuffer.h>
#include <Hubie/ImGui/ImGuiHelpers.h>
#include <imgui/imgui.h>

namespace Hubie
{
	ApplicationInfoWindow::ApplicationInfoWindow()
	{
		m_Name = "ApplicationInfo";
		m_SimpleName = "ApplicationInfo";
	}

	void ApplicationInfoWindow::OnImGui()
	{
		auto flags = ImGuiWindowFlags_NoCollapse;
		ImGui::Begin(m_Name.c_str(), &m_Active, flags);
		{
			if(ImGui::TreeNode("Application"))
			{
				auto systems = Application::Get().GetSystemManager();

				if(ImGui::TreeNode("Systems"))
				{
					systems->OnImGui();
					ImGui::TreePop();
				}

				auto renderGraph = Application::Get().GetRenderGraph();
				if(ImGui::TreeNode("RenderGraph"))
				{
                    renderGraph->OnImGui();
					ImGui::TreePop();
				}

				ImGui::NewLine();
				ImGui::Text("FPS : %5.2i", Engine::Get().Statistics().FramesPerSecond);
				ImGui::Text("UPS : %5.2i", Engine::Get().Statistics().UpdatesPerSecond);
				ImGui::Text("Frame Time : %5.2f ms", Engine::Get().Statistics().FrameTime);
				ImGui::NewLine();
				ImGui::Text("Scene : %s", Application::Get().GetSceneManager()->GetCurrentScene()->GetSceneName().c_str());

				if(ImGui::TreeNode("GBuffer"))
				{
					if(ImGui::TreeNode("Colour Texture"))
					{
						ImGuiHelpers::Image(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_COLOUR), Maths::Vector2(128.0f, 128.0f));
						ImGuiHelpers::Tooltip(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_COLOUR), Maths::Vector2(256.0f, 256.0f));

						ImGui::TreePop();
					}
					if(ImGui::TreeNode("Normal Texture"))
					{
						ImGuiHelpers::Image(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_NORMALS), Maths::Vector2(128.0f, 128.0f));
						ImGuiHelpers::Tooltip(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_NORMALS), Maths::Vector2(256.0f, 256.0f));

						ImGui::TreePop();
					}
					if(ImGui::TreeNode("PBR Texture"))
					{
						ImGuiHelpers::Image(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_PBR), Maths::Vector2(128.0f, 128.0f));
						ImGuiHelpers::Tooltip(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_PBR), Maths::Vector2(256.0f, 256.0f));

						ImGui::TreePop();
					}
					if(ImGui::TreeNode("Position Texture"))
					{
						ImGuiHelpers::Image(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_POSITION), Maths::Vector2(128.0f, 128.0f));
						ImGuiHelpers::Tooltip(Application::Get().GetRenderGraph()->GetGBuffer()->GetTexture(Graphics::SCREENTEX_POSITION), Maths::Vector2(256.0f, 256.0f));

						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			};
		}
		ImGui::End();
	}
}
