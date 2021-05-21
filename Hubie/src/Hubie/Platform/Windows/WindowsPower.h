#pragma once

#include "Hubie/Core/OS/OS.h"

namespace Hubie
{
    class WindowsPower
    {
    public:
        WindowsPower();
        virtual ~WindowsPower();

        PowerState GetPowerState();
        int GetPowerSecondsLeft();
        int GetPowerPercentageLeft();

    private:
        int m_NumberSecondsLeft;
        int m_PercentageLeft;
        PowerState m_PowerState;

        bool GetPowerInfo_Windows(/*PowerState * state, int *seconds, int *percent*/);
        bool UpdatePowerInfo();
    };
}
