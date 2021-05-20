#pragma once

#include "Hubie/Core/OS/KeyCodes.h"

#include <GLFW/glfw3.h>

#include <map>

namespace Hubie
{
	namespace GLFWKeyCodes
	{
		static Hubie::InputCode::Key GLFWToHubieKeyboardKey(uint32_t glfwKey)
		{
			static std::map<uint32_t, Hubie::InputCode::Key> keyMap = {
				{ GLFW_KEY_A, Hubie::InputCode::Key::A },
				{ GLFW_KEY_B, Hubie::InputCode::Key::B },
				{ GLFW_KEY_C, Hubie::InputCode::Key::C },
				{ GLFW_KEY_D, Hubie::InputCode::Key::D },
				{ GLFW_KEY_E, Hubie::InputCode::Key::E },
				{ GLFW_KEY_F, Hubie::InputCode::Key::F },
				{ GLFW_KEY_G, Hubie::InputCode::Key::G },
				{ GLFW_KEY_H, Hubie::InputCode::Key::H },
				{ GLFW_KEY_I, Hubie::InputCode::Key::I },
				{ GLFW_KEY_J, Hubie::InputCode::Key::J },
				{ GLFW_KEY_K, Hubie::InputCode::Key::K },
				{ GLFW_KEY_L, Hubie::InputCode::Key::L },
				{ GLFW_KEY_M, Hubie::InputCode::Key::M },
				{ GLFW_KEY_N, Hubie::InputCode::Key::N },
				{ GLFW_KEY_O, Hubie::InputCode::Key::O },
				{ GLFW_KEY_P, Hubie::InputCode::Key::P },
				{ GLFW_KEY_Q, Hubie::InputCode::Key::Q },
				{ GLFW_KEY_R, Hubie::InputCode::Key::R },
				{ GLFW_KEY_S, Hubie::InputCode::Key::S },
				{ GLFW_KEY_T, Hubie::InputCode::Key::T },
				{ GLFW_KEY_U, Hubie::InputCode::Key::U },
				{ GLFW_KEY_V, Hubie::InputCode::Key::V },
				{ GLFW_KEY_W, Hubie::InputCode::Key::W },
				{ GLFW_KEY_X, Hubie::InputCode::Key::X },
				{ GLFW_KEY_Y, Hubie::InputCode::Key::Y },
				{ GLFW_KEY_Z, Hubie::InputCode::Key::Z },

				{ GLFW_KEY_0, Hubie::InputCode::Key::D0 },
				{ GLFW_KEY_1, Hubie::InputCode::Key::D1 },
				{ GLFW_KEY_2, Hubie::InputCode::Key::D2 },
				{ GLFW_KEY_3, Hubie::InputCode::Key::D3 },
				{ GLFW_KEY_4, Hubie::InputCode::Key::D4 },
				{ GLFW_KEY_5, Hubie::InputCode::Key::D5 },
				{ GLFW_KEY_6, Hubie::InputCode::Key::D6 },
				{ GLFW_KEY_7, Hubie::InputCode::Key::D7 },
				{ GLFW_KEY_8, Hubie::InputCode::Key::D8 },
				{ GLFW_KEY_9, Hubie::InputCode::Key::D9 },

				{ GLFW_KEY_F1, Hubie::InputCode::Key::F1 },
				{ GLFW_KEY_F2, Hubie::InputCode::Key::F2 },
				{ GLFW_KEY_F3, Hubie::InputCode::Key::F3 },
				{ GLFW_KEY_F4, Hubie::InputCode::Key::F4 },
				{ GLFW_KEY_F5, Hubie::InputCode::Key::F5 },
				{ GLFW_KEY_F6, Hubie::InputCode::Key::F6 },
				{ GLFW_KEY_F7, Hubie::InputCode::Key::F7 },
				{ GLFW_KEY_F8, Hubie::InputCode::Key::F8 },
				{ GLFW_KEY_F9, Hubie::InputCode::Key::F9 },
				{ GLFW_KEY_F10, Hubie::InputCode::Key::F10 },
				{ GLFW_KEY_F11, Hubie::InputCode::Key::F11 },
				{ GLFW_KEY_F12, Hubie::InputCode::Key::F12 },

				{ GLFW_KEY_MINUS, Hubie::InputCode::Key::Minus },
				{ GLFW_KEY_DELETE, Hubie::InputCode::Key::Delete },
				{ GLFW_KEY_SPACE, Hubie::InputCode::Key::Space },
				{ GLFW_KEY_LEFT, Hubie::InputCode::Key::Left },
				{ GLFW_KEY_RIGHT, Hubie::InputCode::Key::Right },
				{ GLFW_KEY_UP, Hubie::InputCode::Key::Up },
				{ GLFW_KEY_DOWN, Hubie::InputCode::Key::Down },
				{ GLFW_KEY_LEFT_SHIFT, Hubie::InputCode::Key::LeftShift },
				{ GLFW_KEY_RIGHT_SHIFT, Hubie::InputCode::Key::RightShift },
				{ GLFW_KEY_ESCAPE, Hubie::InputCode::Key::Escape },
				{ GLFW_KEY_KP_ADD, Hubie::InputCode::Key::A },
				{ GLFW_KEY_COMMA, Hubie::InputCode::Key::Comma },
				{ GLFW_KEY_BACKSPACE, Hubie::InputCode::Key::Backspace },
				{ GLFW_KEY_ENTER, Hubie::InputCode::Key::Enter },
				{ GLFW_KEY_LEFT_SUPER, Hubie::InputCode::Key::LeftSuper },
				{ GLFW_KEY_RIGHT_SUPER, Hubie::InputCode::Key::RightSuper },
				{ GLFW_KEY_LEFT_ALT, Hubie::InputCode::Key::LeftAlt },
				{ GLFW_KEY_RIGHT_ALT, Hubie::InputCode::Key::RightAlt },
				{ GLFW_KEY_LEFT_CONTROL, Hubie::InputCode::Key::LeftControl },
				{ GLFW_KEY_RIGHT_CONTROL, Hubie::InputCode::Key::RightControl }
			};

			return keyMap[glfwKey];
		}

		static Hubie::InputCode::MouseKey GLFWToHubieMouseKey(uint32_t glfwKey)
		{

			static std::map<uint32_t, Hubie::InputCode::MouseKey> keyMap = {
				{ GLFW_MOUSE_BUTTON_LEFT, Hubie::InputCode::MouseKey::ButtonLeft },
				{ GLFW_MOUSE_BUTTON_RIGHT, Hubie::InputCode::MouseKey::ButtonRight },
				{ GLFW_MOUSE_BUTTON_MIDDLE, Hubie::InputCode::MouseKey::ButtonMiddle }
			};

			return keyMap[glfwKey];
		}
	}
}

