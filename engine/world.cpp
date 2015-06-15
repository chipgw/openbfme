#include "world.hpp"
#include "object.hpp"
#include "log.hpp"

namespace OpenBFME {

GameWorld::GameWorld() {

}

void GameWorld::tick(decimal delta) {
    Log::debug("World ticked. Delta: %f", delta);

    for (auto object : objects) {
        object->tick(delta);
    }
}

std::weak_ptr<GameObject> GameWorld::createObject() {
    objects.emplace_back(new GameObject(*this));
    return objects.back();
}

}
