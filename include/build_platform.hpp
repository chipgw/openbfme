#ifndef OPENBFME_BUILD_PLATFORM_HPP
#define OPENBFME_BUILD_PLATFORM_HPP

#ifdef _MSC_VER
#include "build_msvc.hpp"
#else
/* TODO - add other platforms and error if unsupported. */
#define EXPORT
#endif

#endif //OPENBFME_BUILD_PLATFORM_HPP
