#include "gameplayer.hpp"
#include "gameworld.hpp"
#include "iniobject.hpp"
#include "log.hpp"

namespace OpenBFME {

GamePlayer::GamePlayer(GameWorld& owner, const IniObject& plrTemplate)
    : world(owner), playerTemplate(plrTemplate), playerMoney(0) {
    Log::debug("Created player with template: \"%s\"", playerTemplate.args[0]);

    auto startMoney = playerTemplate.variables.find("StartMoney");

    if (startMoney != playerTemplate.variables.end())
        Log::debug("Player starting money: %i", playerMoney = startMoney->second.i);

    auto startBuilding = playerTemplate.variables.find("StartingBuilding");

    if (startBuilding != playerTemplate.variables.end())
        /* TODO - Starting position. */
        world.createObject(startBuilding->second.s, *this);

    /* TODO - More stuffs. */
}

}
