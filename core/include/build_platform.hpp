#pragma once

#define IN_BUILD_PLATFORM_HPP

#if defined(_WIN32)
#include "build_windows.hpp"
#elif defined(__linux)
#include "build_linux.hpp"
#else
/* TODO - Support more platforms. */
#error Unsupported platform!
#endif

#undef IN_BUILD_PLATFORM_HPP
