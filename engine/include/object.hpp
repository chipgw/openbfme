#pragma once

#include "types.hpp"

namespace OpenBFME {

class GameObject {
    GameWorld& world;
public:
    GameObject(GameWorld& w);

    void tick(decimal delta);
};

}
