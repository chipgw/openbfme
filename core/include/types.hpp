#pragma once

#include "build_platform.hpp"
#include <string>
#include <cstdint>
#include <glm/vec4.hpp>

namespace OpenBFME {

typedef int32_t integer;
typedef float decimal;

typedef char character;

typedef const character* cstring;
typedef std::basic_string<character> string;

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

class BigEntry;
class BigArchive;

struct IniVariable;
class IniType;
struct IniObject;
class IniParser;

class GameObject;
class GameWorld;

}
