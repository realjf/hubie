#pragma once
#include "Hubie/Maths/Maths.h"
#include "Hubie/Events/Event.h"
#include "Hubie/Events/KeyEvent.h"
#include "Hubie/Events/MouseEvent.h"
#include "Hubie/Utilities/TSingleton.h"
#include "KeyCodes.h"

#define MAX_KEYS 1024
#define MAX_BUTTONS 32

namespace Hubie
{
	class Event;

	enum class MouseMode
	{
		Visible,
		Hidden,
		Captured
	};

	class HB_EXPORT Input : public ThreadSafeSingleton<Input>
	{
		friend class TSingleton<Input>;

	public:
		Input();
		virtual ~Input() = default;

		bool GetKeyPressed(Hubie::InputCode::Key key) const { return m_KeyPressed[int(key)]; }
		bool GetKeyHeld(Hubie::InputCode::Key key) const { return m_KeyHeld[int(key)]; }
		bool GetMouseClicked(Hubie::InputCode::MouseKey key) const { return m_MouseClicked[int(key)]; }
		bool GetMouseHeld(Hubie::InputCode::MouseKey key) const { return m_MouseHeld[int(key)]; }

		void SetKeyPressed(Hubie::InputCode::Key key, bool a) { m_KeyPressed[int(key)] = a; }
		void SetKeyHeld(Hubie::InputCode::Key key, bool a) { m_KeyHeld[int(key)] = a; }
		void SetMouseClicked(Hubie::InputCode::MouseKey key, bool a) { m_MouseClicked[int(key)] = a; }
		void SetMouseHeld(Hubie::InputCode::MouseKey key, bool a) { m_MouseHeld[int(key)] = a; }

		void SetMouseOnScreen(bool onScreen) { m_MouseOnScreen = onScreen; }
		bool GetMouseOnScreen() const { return m_MouseOnScreen; }

		void StoreMousePosition(float xpos, float ypos) { m_MousePosition = Maths::Vector2(float(xpos), float(ypos)); }
		const Maths::Vector2& GetMousePosition() const { return m_MousePosition; }

		void SetWindowFocus(bool focus) { m_WindowFocus = focus; }
		bool GetWindowFocus() const { return m_WindowFocus; }

		void SetScrollOffset(float offset) { m_ScrollOffset = offset; }
		float GetScrollOffset() const { return m_ScrollOffset; }

		void Reset();
		void ResetPressed();
		void OnEvent(Event& e);

		MouseMode GetMouseMode() const { return m_MouseMode; }
		void SetMouseMode(MouseMode mode) { m_MouseMode = mode; }

	protected:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);
		bool OnMouseReleased(MouseButtonReleasedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseEnter(MouseEnterEvent& e);

		bool m_KeyPressed[MAX_KEYS];
		bool m_KeyHeld[MAX_KEYS];

		bool m_MouseHeld[MAX_BUTTONS];
		bool m_MouseClicked[MAX_BUTTONS];

		float m_ScrollOffset = 0.0f;

		bool m_MouseOnScreen;
		bool m_WindowFocus;

		MouseMode m_MouseMode;

		Maths::Vector2 m_MousePosition;
	};
}
