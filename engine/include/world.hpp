#pragma once

#include "types.hpp"
#include <memory>
#include <list>

namespace OpenBFME {

class GameWorld {
    std::list<std::shared_ptr<GameObject>> objects;

public:
    /* TODO - Make it only creatable by specific means. */
    EXPORT GameWorld();

    EXPORT void tick(decimal delta);

    /* TODO - Arguments. */
    EXPORT std::weak_ptr<GameObject> createObject();
};

}
