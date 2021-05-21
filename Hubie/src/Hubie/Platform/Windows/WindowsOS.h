
#include "Hubie/Core/OS/OS.h"

namespace Hubie
{
    class HB_EXPORT WindowsOS : public OS
    {
    public:
        WindowsOS() = default;
        ~WindowsOS() = default;

        void Init();
        void Run() override;
        std::string GetExecutablePath() override
        {
            return "";
        }
    };
}