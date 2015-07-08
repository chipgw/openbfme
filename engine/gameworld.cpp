#include "gameworld.hpp"
#include "gameobject.hpp"
#include "log.hpp"
#include "iniobject.hpp"
#include <algorithm>

namespace OpenBFME {

GameWorld::GameWorld(const IniObject& root): iniRoot(root) {
    /* TODO - Put stuff here. */
}

void GameWorld::tick(decimal delta) {
    Log::debug("World ticked. Delta: %f", delta);

    for (auto object : objects) {
        object->tick(delta);
    }
}

std::weak_ptr<GameObject> GameWorld::createObject(const string& templateName, const string& objectName) {
    /* Find an Object tempolate with a matching name. */
    auto objTemplate = std::find_if(iniRoot.subObjects.cbegin(), iniRoot.subObjects.cend(), [&](const std::pair<string, IniObject> def) {
        return def.first == "Object" && !def.second.args.empty() && def.second.args[0] == templateName;
    });

    /* If a template was found, create the Object. */
    if (objTemplate != iniRoot.subObjects.cend()) {
        objects.emplace_back(new GameObject(*this, objTemplate->second, objectName));

        Log::debug("Object created with template \"%s\".", templateName);

        return objects.back();
    }

    /* If no template was found return an empty weak_ptr. */
    return std::weak_ptr<GameObject>();
}

}
