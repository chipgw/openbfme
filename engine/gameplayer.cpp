#include "gameplayer.hpp"
#include "gameworld.hpp"
#include "iniobject.hpp"
#include "log.hpp"

namespace OpenBFME {

GamePlayer::GamePlayer(GameWorld& owner, const IniObject& plrTemplate)
    : world(owner), playerTemplate(plrTemplate), playerMoney(0) {
    Log::debug("Created player with template: \"%s\"", playerTemplate.args[0]);

    Log::debug("Player starting money: %i", playerMoney = playerTemplate.getIntegerVariable("StartMoney"));

    string startBuilding = playerTemplate.getStringVariable("StartingBuilding");

    if (startBuilding.size())
        /* TODO - Starting position. */
        world.createObject(startBuilding, *this);

    /* TODO - More stuffs. */
}

}
