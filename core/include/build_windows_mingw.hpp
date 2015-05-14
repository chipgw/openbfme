/* No header guard because we prevent including outside of "build_platform.hpp", which does contain a header guard. */
#ifndef IN_BUILD_PLATFORM_HPP
#error "build_windows_mingw.hpp" should not be included directly! use "build_platform.hpp"!
#endif

/* If this header is included we define MinGW related stuff. */
#define OPENBFME_COMPILER_MINGW

#define EXPORT

/* TODO - Once the std filesystem is out use that. */
#define FILESYSTEM_HEADER <boost/filesystem.hpp>
#define FILESYSTEM_NAMESPACE boost::filesystem
