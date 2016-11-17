#include "gameplayer.hpp"
#include "gameworld.hpp"
#include "iniobject.hpp"
#include "langstrings.hpp"
#include "log.hpp"

namespace OpenBFME {

GamePlayer::GamePlayer(GameWorld& owner, const IniObject& plrTemplate)
    : world(owner), playerTemplate(plrTemplate), playerMoney(0) {
    playerMoney = playerTemplate.getIntegerVariable("StartMoney");

    string startBuilding = playerTemplate.getStringVariable("StartingBuilding");

    if (startBuilding.size())
        /* TODO - Starting position. */
        world.createObject(startBuilding, *this);

    factionDisplayName = LangStrings::getString(playerTemplate.getStringVariable("DisplayName"));

    isEvil = playerTemplate.getBooleanVariable("Evil");

    Log::debug("Created a%s player with template: \"%s\" display name \"%s\" starting money: %i",
               isEvil ? "n evil" : " good", playerTemplate.args[0], factionDisplayName, playerMoney);

    /* TODO - More stuffs. */
}

}
