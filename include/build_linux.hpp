/* No header guard because we prevent including outside of "build_platform.hpp", which does contain a header guard. */
#ifndef IN_BUILD_PLATFORM_HPP
#error "build_linux.hpp" should not be included directly! use "build_platform.hpp"!
#endif

/* If this header is included we define Linux related stuff. */
#define OPENBFME_PLATFORM_LINUX

#define EXPORT

/* TODO - Once the std filesystem is out use that. */
#define FILESYSTEM_HEADER <boost/filesystem.hpp>
#define FILESYSTEM_NAMESPACE boost::filesystem
