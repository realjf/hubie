#pragma once

#include "Hubie/Core/Core.h"
#include "Hubie/Events/KeyEvent.h"
#include "Hubie/Events/Event.h"
#include "Hubie/Events/MouseEvent.h"
#include "Hubie/Events/ApplicationEvent.h"

namespace Hubie
{
    class Scene;
    class TimeStep;
    namespace Graphics
    {
        class IMGUIRenderer;
    }

    class HB_EXPORT ImGuiManager
    {
    public:
        ImGuiManager(bool clearScreen = false);
        ~ImGuiManager();

        void OnInit();
        void OnUpdate(const TimeStep& dt, Scene* scene);
        void OnEvent(Event& event);
        void OnRender(Scene* scene);
        void OnNewScene(Scene* scene);

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnwindowResizeEvent(WindowResizeEvent& e);

        void SetImGuiKeyCodes();
        void SetImGuiStyle();
        void AddIconFont();

        float m_FontSize;
        float m_DPIScale;

        UniqueRef<Graphics::IMGUIRenderer> m_IMGUIRenderer;
        bool m_ClearScreen;
    };

}
