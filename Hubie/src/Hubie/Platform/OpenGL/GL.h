#pragma once

#ifdef HB_PLATFORM_MOBILE
#ifdef HB_PLATFORM_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#elif HB_PLATFORM_ANDROID
#include <GLES3/gl3.h>
#endif

#else
#ifdef HB_PLATFORM_WINDOWS
#include <glad/glad.h>
#elif HB_PLATFORM_LINUX
#include <glad/glad.h>
#elif HB_PLATFORM_MACOS
#include <glad/glad.h>
#include <OpenGL/gl.h>
#endif

#endif
