#pragma once

#include "Core.h"

#ifdef HB_PLATFORM_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Hubie
{
    class HB_EXPORT Log
    {
    public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
}

#define HB_TRACE(...) SPDLOG_LOGGER_CALL(::Hubie::Log::GetCoreLogger(), spdlog::level::level_enum::trace, __VA_ARGS__)
#define HB_INFO(...) SPDLOG_LOGGER_CALL(::Hubie::Log::GetCoreLogger(), spdlog::level::level_enum::info, __VA_ARGS__)
#define HB_WARN(...) SPDLOG_LOGGER_CALL(::Hubie::Log::GetCoreLogger(), spdlog::level::level_enum::warn, __VA_ARGS__)
#define HB_ERROR(...) SPDLOG_LOGGER_CALL(::Hubie::Log::GetCoreLogger(), spdlog::level::level_enum::err, __VA_ARGS__)
#define HB_CRITICAL(...) SPDLOG_LOGGER_CALL(::Hubie::Log::GetCoreLogger(), spdlog::level::level_enum::critical, __VA_ARGS__)

