#pragma once

#include "types.hpp"

namespace OpenBFME {

class GameObject {
    /* The world the object is contained in. */
    GameWorld& world;

    /* The IniObject that defines this object's behavior. */
    const IniObject& objectTemplate;
    
public:
    /* The name of the object, for script references. Can be empty. */
    const string name;

    vec3 location;
    float angle;
    float scale;

    GameObject(GameWorld& owner, const IniObject& objTemplate, const string& objName = "");

    void tick(decimal delta);
};

}
