#ifndef OPENBFME_BUILD_MSVC_HPP
#define OPENBFME_BUILD_MSVC_HPP

/* for _mkdir() */
#include <direct.h>
#define mkdir _mkdir

#define EXPORT __declspec(dllexport)

#endif //OPENBFME_BUILD_MSVC_HPP
