#include "object.hpp"
#include "log.hpp"

namespace OpenBFME {

GameObject::GameObject(GameWorld& owner, const IniObject& objTemplate, const string& objName) 
    : world(owner), objectTemplate(objTemplate), name(objName) {
    /* TODO - init stuff. */
}

void GameObject::tick(decimal delta) {
    Log::debug("Object ticked. Object Name: \"%s\" Delta: %f", name, delta);
}

}
