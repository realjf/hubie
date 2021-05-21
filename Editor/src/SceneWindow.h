
#include "EditorWindow.h"
#include "Editor.h"
#include <Hubie/Maths/Maths.h>
#include <Hubie/Maths/Frustum.h>
#include <Hubie/Maths/Transform.h>
#include <Hubie/Graphics/Camera/Camera.h>
#include <Hubie/Core/StringUtilities.h>
#include <Hubie/ImGui/ImGuiHelpers.h>

#include <imgui/imgui.h>
DISABLE_WARNING_PUSH
DISABLE_WARNING_CONVERSION_TO_SMALLER_TYPE
#include <entt/entt.hpp>
DISABLE_WARNING_POP

namespace Hubie
{
	class SceneWindow : public EditorWindow
	{
	public:
		SceneWindow();
		~SceneWindow() = default;

		void OnImGui() override;
		void OnNewScene(Scene* scene) override;
        void ToolBar();
		void DrawGizmos(float width, float height, float xpos, float ypos, Scene* scene);

		void Resize(uint32_t width, uint32_t height);
	private:

		template<typename T>
		void ShowComponentGizmo(float width, float height, float xpos, float ypos, const Maths::Matrix4& viewProj, const Maths::Frustum& frustum, entt::registry& registry)
		{
			if (m_ShowComponentGizmoMap[typeid(T).hash_code()])
			{
				auto group = registry.group<T>(entt::get<Maths::Transform>);

				for (auto entity : group)
				{
                    const auto &[component, trans] = group.template get<T, Maths::Transform>(entity);
					
					Maths::Vector3 pos = trans. GetWorldPosition();

					auto inside = frustum.IsInside(pos);

                    if (inside == Maths::Intersection::OUTSIDE)
                        continue;
                    
                    Maths::Vector2 screenPos = Maths::WorldToScreen(pos, viewProj, width, height, xpos, ypos);
                    ImGui::SetCursorPos({ screenPos.x - ImGui::GetFontSize() / 2.0f , screenPos.y - ImGui::GetFontSize() / 2.0f });
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.7f, 0.7f, 0.0f));

                    if(ImGui::Button(m_Editor->GetComponentIconMap()[typeid(T).hash_code()]))
                    {
                        m_Editor->SetSelected(entity);
                    }
                
                    ImGui::PopStyleColor();

                    ImGuiHelpers::Tooltip(StringUtilities::Demangle(typeid(T).name()));
				}
			}
		}

		std::unordered_map<size_t, bool> m_ShowComponentGizmoMap;

		bool m_ShowStats = false;
        Ref<Graphics::Texture2D> m_GameViewTexture = nullptr;
		Scene* m_CurrentScene = nullptr;
		float m_AspectRatio;
		uint32_t m_Width, m_Height;
        
        bool m_FreeAspect = true;
        float m_FixedAspect = 1.0f;
        bool m_HalfRes = false;
	};
}
