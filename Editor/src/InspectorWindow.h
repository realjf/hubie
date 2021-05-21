#pragma once

#include "EditorWindow.h"
#include <Hubie/ImGui/ImGuiEnttEditor.hpp>

namespace Hubie
{
	class InspectorWindow : public EditorWindow
	{
	public:
		InspectorWindow();
		~InspectorWindow() = default;

		void OnNewScene(Scene* scene) override;
		void OnImGui() override;
		void SetDebugMode(bool mode);
		bool GetIsDebugMode() const { return m_DebugMode; };

	private:
		MM::ImGuiEntityEditor<entt::entity> m_EnttEditor;
		bool m_DebugMode = false;
	};
}
