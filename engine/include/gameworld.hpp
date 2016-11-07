#pragma once

#include "types.hpp"
#include "gameplayer.hpp"
#include <memory>
#include <list>

namespace OpenBFME {

class GameWorld {
    std::list<GamePlayer> players;
    std::list<std::shared_ptr<GameObject>> objects;

    /* The root IniObject that containe object definitions and such. */
    const IniObject& iniRoot;

public:
    /* TODO - Make it only creatable by specific means. */
    EXPORT GameWorld(const IniObject& root);

    EXPORT void tick(decimal delta);

    /* Create a player for use in the world. */
    EXPORT GamePlayer* createPlayer(const string& playerTemplateName);

    /* Create an object and add it to the world. */
    /* TODO - More arguments. */
    EXPORT std::weak_ptr<GameObject> createObject(const string& templateName, GamePlayer& player, const string& objectName = "");
};

}
