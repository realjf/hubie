#pragma once
#include "Hubie/Core/OS/OS.h"

namespace Hubie
{
    class UnixOS : public OS
    {
    public:
        UnixOS() = default;
        ~UnixOS() = default;

        void Init();
        void Run() override;
        std::string GetExecutablePath() override
        {
            return "";
        }
    };
}
