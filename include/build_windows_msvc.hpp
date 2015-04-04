/* No header guard because we prevent including outside of "build_platform.hpp", which does contain a header guard. */
#ifndef IN_BUILD_PLATFORM_HPP
#error "build_windows_msvc.hpp" should not be included directly! use "build_platform.hpp"!
#endif

/* If this header is included we define MSVC related stuff. */
#define OPENBFME_COMPILER_MSVC

/* I'm sick of MSVC warning me that fopen() is unsafe. */
#define _CRT_SECURE_NO_WARNINGS

#define EXPORT __declspec(dllexport)

#define FILESYSTEM_HEADER <filesystem>
/* TODO - This namespace is temporary, as this functionality isn't final yet. */
#define FILESYSTEM_NAMESPACE std::tr2::sys

#if _MSC_VER == 1800
/* These name differences are fixed in VS 2015. */
#define canonical complete
#define generic_string string
#endif
