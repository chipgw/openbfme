#pragma once

#include "types.hpp"

namespace OpenBFME {

class GameWorld {
    GameWorld();

    EXPORT void tick(decimal delta);
};

}
