#ifndef OPENBFME_TYPES_HPP
#define OPENBFME_TYPES_HPP

#include "build_platform.hpp"
#include <string>
#include <cstdint>

namespace OpenBFME {

typedef int integer;
typedef float decimal;
typedef char character;
typedef const character* cstring;
typedef std::basic_string<character> string;

}

#endif //OPENBFME_TYPES_HPP
