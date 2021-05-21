#include "Hubie/Core/OS/OS.h"

namespace Hubie
{
    class MacOSOS : public OS
    {
    public:
        MacOSOS()
        {
        }
        ~MacOSOS()
        {
        }

        void Init();
        void Run() override;
        std::string GetExecutablePath() override;
        void SetTitleBarColour(const Maths::Vector4& colour, bool dark = true) override;
    };
}
