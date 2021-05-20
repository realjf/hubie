#pragma once

#if HB_PROFILE
#ifdef HB_PLATFORM_WINDOWS
#define TRACY_CALLSTACK 1
#endif
#include <Tracy/Tracy.hpp>
#define HB_PROFILE_SCOPE(name) ZoneScopedN(name)
#define HB_PROFILE_FUNCTION() ZoneScoped
#define HB_PROFILE_FRAMEMARKER() FrameMark
#define HB_PROFILE_LOCK(type, var, name) TracyLockableN(type, var, name)
#define HB_PROFILE_LOCKMARKER(var) LockMark(var)
#define HB_PROFILE_SETTHREADNAME(name) tracy::SetThreadName(name)
#else
#define HB_PROFILE_SCOPE(name)
#define HB_PROFILE_FUNCTION()
#define HB_PROFILE_FRAMEMARKER()
#define HB_PROFILE_LOCK(type, var, name) type var
#define HB_PROFILE_LOCKMARKER(var)
#define HB_PROFILE_SETTHREADNAME(name)

#endif
