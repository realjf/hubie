#ifndef __CORE_H__
#define __CORE_H__




#ifdef HB_ENABLE_ASSERTS
    #define HB_ASSERT(x, ...) { if(!(x)) {HB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
    #define HB_CORE_ASSERT(x, ...) { if(!(x)){ HB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
    #define HB_ASSERT(x, ...)
    #define HB_CORE_ASSERT(x, ...)
#endif

#endif
