#include "object.hpp"
#include "log.hpp"

namespace OpenBFME {

GameObject::GameObject(GameWorld& w) : world(w) {

}

void GameObject::tick(decimal delta) {
    Log::debug("Object ticked. Delta: %f", delta);
}

}
