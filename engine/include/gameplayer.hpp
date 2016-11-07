#pragma once

#include "types.hpp"

namespace OpenBFME {

class GamePlayer {
    /* The IniObject that defines this player settings. */
    const IniObject& playerTemplate;

public:
    /* The world the player is contained in. */
    GameWorld& world;

    /* Only callable by GameWorld. */
    GamePlayer(GameWorld& owner, const IniObject& plrTemplate);

    /* How much money/resources the player has. */
    integer playerMoney;
};

}
