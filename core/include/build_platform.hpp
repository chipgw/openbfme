#pragma once

#define IN_BUILD_PLATFORM_HPP

#if defined(_WIN32)
/* Begin Windows */
#define OPENBFME_PLATFORM_WINDOWS
#if defined(_MSC_VER) && _MSC_VER >= 1800
#include "build_windows_msvc.hpp"
#elif defined(__MINGW32__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 8
#include "build_windows_mingw.hpp"
#else
/* TODO - Possibly support some other compilers. */
#error Only Visual Studio 2013, Visual Studio 2015, and MinGW >= 4.8 are supported on Windows!
#endif
/* End Windows */
#elif defined(__linux)
#include "build_linux.hpp"
#else
/* TODO - Support more platforms. */
#error Unsupported platform!
#endif

#undef IN_BUILD_PLATFORM_HPP
