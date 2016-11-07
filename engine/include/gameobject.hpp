#pragma once

#include "types.hpp"

namespace OpenBFME {

class GameObject {
    /* The IniObject that defines this object's behavior. */
    const IniObject& objectTemplate;

public:
    /* The world the object is contained in. */
    GameWorld& world;

    /* The player that the object belongs to. */
    GamePlayer& player;

    /* The name of the object, for script references. Can be empty. */
    const string name;

    vec3 location;
    float angle;
    float scale;

    /* Not to be called anywhere other than in GameWorld.createObject() */
    GameObject(GamePlayer& owner, const IniObject& objTemplate, const string& objName = "");

    /* Called once per frame. */
    void tick(decimal delta);
};

}
