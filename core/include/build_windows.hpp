/* No header guard because we prevent including outside of "build_platform.hpp", which does contain a header guard. */
#ifndef IN_BUILD_PLATFORM_HPP
#error "build_windows.hpp" should not be included directly! use "build_platform.hpp"!
#endif

#define OPENBFME_PLATFORM_WINDOWS

#if defined(_MSC_VER) && _MSC_VER >= 1800
/* ----------- *
 * Begin MSVC. *
 * ----------- */
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

/* --------- *
 * End MSVC. *
 * --------- */

#elif defined(__MINGW32__) && __GNUC__ == 4 && __GNUC_MINOR__ >= 8
/* ------------ *
 * Begin MinGW. *
 * ------------ */
#define OPENBFME_COMPILER_MINGW

#define EXPORT

/* TODO - Once the std filesystem is out use that. */
#define FILESYSTEM_HEADER <boost/filesystem.hpp>
#define FILESYSTEM_NAMESPACE boost::filesystem

/* ---------- *
 * End MinGW. *
 * ---------- */

#else
/* TODO - Possibly support some other compilers. */
#error Only Visual Studio 2013, Visual Studio 2015, and MinGW >= 4.8 are supported on Windows!
#endif
