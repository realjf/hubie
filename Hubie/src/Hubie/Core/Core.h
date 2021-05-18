#pragma once


#ifndef HB_PLATFORM_WINDOWS
#include <signal.h>
#else
#include <Windows.h>
#endif // !HB_PLATFORM_WINDOWS

#include <stdint.h>


#ifdef HB_PLATFORM_WINDOWS

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
#define LUMOS_EXPORT __attribute__((visibility("default")))
#define LUMOS_HIDDEN __attribute__((visibility("hidden")))
#endif


#define BIT(x) (1 << x)

#ifdef HB_ENABLE_ASSERTS
    #define HB_ASSERT(x, ...) { if(!(x)) {HB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
    #define HB_CORE_ASSERT(x, ...) { if(!(x)){ HB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
    #define HB_ASSERT(x, ...)
    #define HB_CORE_ASSERT(x, ...)
#endif


