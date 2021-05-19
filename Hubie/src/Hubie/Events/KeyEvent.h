#pragma once

#include "Hubie/Core/Core.h"
#include "Event.h"
#include "Hubie/Core/OS/KeyCodes.h"

#include <sstream>

namespace Hubie
{

    class HB_EXPORT KeyEvent : public Event
    {
    public:
        inline Hubie::InputCode::Key GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(Hubie::InputCode::Key keycode)
            : m_KeyCode(keycode)
        {
        }

        Hubie::InputCode::Key m_KeyCode;
    };

    class HB_EXPORT KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(Hubie::InputCode::Key keycode, int repeatCount)
            : KeyEvent(keycode)
            , m_RepeatCount(repeatCount)
        {
        }

        inline int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << uint32_t(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class HB_EXPORT KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(Hubie::InputCode::Key keycode)
            : KeyEvent(keycode)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << uint32_t(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class HB_EXPORT KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(Hubie::InputCode::Key keycode, char character)
            : KeyEvent(keycode)
            , Character(character)
        {
        }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << uint32_t(m_KeyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)

        char Character;

    private:
    };
}
