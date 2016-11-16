#include "gameobject.hpp"
#include "gameplayer.hpp"
#include "iniobject.hpp"
#include "langstrings.hpp"
#include "log.hpp"

namespace OpenBFME {

GameObject::GameObject(GamePlayer& owner, const IniObject& objTemplate, const string& objName)
    : world(owner.world), player(owner), objectTemplate(objTemplate), name(objName) {
    /* TODO - What happens if it doesn't exist? Just having an empty string probably isn't the best way to handle it... */
    /* Get the localized string with the specified key. */
    Log::debug("Object display name: \"%s\"", displayName = LangStrings::getString(objectTemplate.getStringVariable("DisplayName")));

    /* TODO - What happens if it doesn't exist? Just having an empty string probably isn't the best way to handle it... */
    /* Get the localized string with the specified key. */
    Log::debug("Object description: \"%s\"", description = LangStrings::getString(objectTemplate.getStringVariable("Description")));

    /* TODO - What happens if it doesn't exist? Getting a decimal from a BadVar is undefined... */
    Log::debug("Object vision range: \"%f\"", visionRange = objectTemplate.getDecimalVariable("VisionRange"));

    /* TODO - What happens if it doesn't exist? Getting a decimal from a BadVar is undefined... */
    Log::debug("Object shroud clear range: \"%f\"", shroudClearRange = objectTemplate.getDecimalVariable("ShroudClearingRange"));

    /* TODO - init stuff. */
}

void GameObject::tick(decimal delta) {
    Log::debug("Object ticked. Object Name: \"%s\" Delta: %f", name, delta);

    /* TODO - There is a heck of a lot of stuff that belongs here... */
}

}
