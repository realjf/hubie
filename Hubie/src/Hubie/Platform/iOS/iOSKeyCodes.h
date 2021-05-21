#pragma once

#include "Hubie/Core/OS/KeyCodes.h"
#include <map>

namespace Hubie
{
    namespace iOSKeyCodes
    {
        inline Hubie::InputCode::Key iOSKeyToHubie(char key)
        {
            static std::map<char, Hubie::InputCode::Key> keyMap = {
                { 'a', Hubie::InputCode::Key::A },
                { 'b', Hubie::InputCode::Key::B },
                { 'c', Hubie::InputCode::Key::C },
                { 'd', Hubie::InputCode::Key::D },
                { 'e', Hubie::InputCode::Key::E },
                { 'f', Hubie::InputCode::Key::F },
                { 'g', Hubie::InputCode::Key::G },
                { 'h', Hubie::InputCode::Key::H },
                { 'i', Hubie::InputCode::Key::I },
                { 'j', Hubie::InputCode::Key::J },
                { 'k', Hubie::InputCode::Key::K },
                { 'l', Hubie::InputCode::Key::L },
                { 'm', Hubie::InputCode::Key::M },
                { 'n', Hubie::InputCode::Key::N },
                { 'o', Hubie::InputCode::Key::O },
                { 'p', Hubie::InputCode::Key::P },
                { 'q', Hubie::InputCode::Key::Q },
                { 'r', Hubie::InputCode::Key::R },
                { 's', Hubie::InputCode::Key::S },
                { 't', Hubie::InputCode::Key::T },
                { 'u', Hubie::InputCode::Key::U },
                { 'v', Hubie::InputCode::Key::V },
                { 'w', Hubie::InputCode::Key::W },
                { 'x', Hubie::InputCode::Key::X },
                { 'y', Hubie::InputCode::Key::Y },
                { 'z', Hubie::InputCode::Key::Z },

                { '0', Hubie::InputCode::Key::D0 },
                { '1', Hubie::InputCode::Key::D1 },
                { '2', Hubie::InputCode::Key::D2 },
                { '3', Hubie::InputCode::Key::D3 },
                { '4', Hubie::InputCode::Key::D4 },
                { '5', Hubie::InputCode::Key::D5 },
                { '6', Hubie::InputCode::Key::D6 },
                { '7', Hubie::InputCode::Key::D7 },
                { '8', Hubie::InputCode::Key::D8 },
                { '9', Hubie::InputCode::Key::D9 },

                { '-', Hubie::InputCode::Key::Minus },
                { 0x75, Hubie::InputCode::Key::Delete },
                { ' ', Hubie::InputCode::Key::Space },
                { 0x7B, Hubie::InputCode::Key::Left },
                { 0x7C, Hubie::InputCode::Key::Right },
                { 0x7E, Hubie::InputCode::Key::Up },
                { 0x7D, Hubie::InputCode::Key::Down },
                { 0x38, Hubie::InputCode::Key::LeftShift },
                { 0x35, Hubie::InputCode::Key::Escape },
                { '+', Hubie::InputCode::Key::Equal },
                { 0x33, Hubie::InputCode::Key::Backspace },
                { 0x24, Hubie::InputCode::Key::Enter },
                { ',', Hubie::InputCode::Key::Comma }
            };

            return keyMap[key];
        }

        inline Hubie::InputCode::MouseKey iOSTouchToHubieMouseKey(uint32_t count)
        {
            if(count > 3)
                return Hubie::InputCode::MouseKey::ButtonLeft;

            static std::map<uint32_t, Hubie::InputCode::MouseKey> keyMap = {
                { 0, Hubie::InputCode::MouseKey::ButtonLeft },
                { 1, Hubie::InputCode::MouseKey::ButtonRight },
                { 2, Hubie::InputCode::MouseKey::ButtonMiddle }
            };
            return keyMap[count];
        }
    }
}
