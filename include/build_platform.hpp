#pragma once

#ifdef _MSC_VER
#include "build_msvc.hpp"
#else
/* TODO - add other platforms and error if unsupported. */
#define OPENBFME_PLATFORM_OTHER

#define EXPORT
#endif
