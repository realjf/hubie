#pragma once


#ifndef HB_PLATFORM_WINDOWS
#include <signal.h>
#else
#include <Windows.h>
#endif // !HB_PLATFORM_WINDOWS

#include <stdint.h>


#if defined(HB_PLATFORM_WINDOWS)

    #ifdef HB_DYNAMIC
        #ifdef HB_ENGINE
        #define HB_EXPORT __declspec(dllexport)
        #else
        #define HB_EXPORT __declspec(dllimport)
        #endif
    #else
        #define HB_EXPORT
    #endif
    
    #define HB_HIDDEN

#else
#define HB_EXPORT __attribute__((visibility("default")))
#define HB_HIDDEN __attribute__((visibility("hidden")))
#endif


#define BIT(x) (1 << x)

#if HB_PLATFORM_WINDOWS
#define HB_BREAK() __debugbreak();
#else
#define HB_BREAK() raise(SIGTRAP);
#endif

#ifdef HB_DEBUG
#define HB_ENABLE_ASSERTS
#endif

#ifdef HB_ENABLE_ASSERTS
#define HB_ASSERT_NO_MESSAGE(condition)        \
    {                                             \
        if(!(condition))                          \
        {                                         \
            HB_ERROR("Assertion Failed!"); \
            HB_BREAK();                        \
        }                                         \
    }

#define HB_ASSERT_MESSAGE(condition, ...)                        \
    {                                                               \
        if(!(condition))                                            \
        {                                                           \
            HB_ERROR("Assertion Failed : {0}", __VA_ARGS__); \
            HB_BREAK();                                          \
        }                                                           \
    }

#define HB_CLIENT_ASSERT HB_ASSERT_MESSAGE
#define HB_CORE_ASSERT HB_ASSERT_MESSAGE
#else
#define HB_CLIENT_ASSERT(...)
#define HB_CORE_ASSERT(...)
#define HB_ASSERT_NO_MESSAGE(...)
#define HB_ASSERT_MESSAGE(condition)
#endif

#ifdef HB_ENGINE
#define HB_ASSERT HB_CORE_ASSERT
#else
#define HB_ASSERT HB_CLIENT_ASSERT
#endif

#define NONCOPYABLE(type_identifier)                  \
    type_identifier(const type_identifier&) = delete; \
    type_identifier& operator=(const type_identifier&) = delete;


