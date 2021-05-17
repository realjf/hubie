#ifndef __LOG_H__
#define __LOG_H__

#include "Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


namespace Hubie
{
    class HUBIE_EXPORT Log
    {
    public:
        static void OnInit();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    }
}


#endif