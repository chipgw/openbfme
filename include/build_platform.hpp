#pragma once

#define IN_BUILD_PLATFORM_HPP

#if defined(WIN32)
/* Begin Windows */
#if defined(_MSC_VER)
#include "build_windows_msvc.hpp"
#else
/* TODO - Support MinGW and possibly some other compilers. */
#error Currently only MSVC is supported on Windows!
#endif
/* End Windows */
#elif defined(__linux)
#include "build_linux.hpp"
#else
/* TODO - Support more platforms. */
#error Unsupported platform!
#endif

#undef IN_BUILD_PLATFORM_HPP
