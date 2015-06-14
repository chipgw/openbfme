#pragma once

#include "types.hpp"

namespace OpenBFME {

class GameObject {
    GameObject();

    EXPORT void tick(decimal delta);
};

}
