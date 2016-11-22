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
    displayName = LangStrings::getString(objectTemplate.getStringVariable("DisplayName"));

    /* TODO - What happens if it doesn't exist? Just having an empty string probably isn't the best way to handle it... */
    /* Get the localized string with the specified key. */
    description = LangStrings::getString(objectTemplate.getStringVariable("Description"));

    /* Default vision & clear range is none. */
    visionRange = objectTemplate.getDecimalVariable("VisionRange", 0.0f);
    shroudClearRange = objectTemplate.getDecimalVariable("ShroudClearingRange", 0.0f);

    auto body = objectTemplate.subObjects.find("Body");

    maxHealth = 0;

    if (body != objectTemplate.subObjects.end()) {
        maxHealth = body->second.getDecimalVariable("MaxHealth");
    }

    /* Start out atmaximum health. */
    health = maxHealth;

    Log::debug("Object display name: \"%s\" description: \"%s\" vision range: %f shroud clear range: %f, health: %f",
               displayName, description, visionRange, shroudClearRange, health);
}

void GameObject::tick(decimal delta) {
    Log::debug("Object ticked. Object Name: \"%s\" Delta: %f", name, delta);

    /* TODO - There is a heck of a lot of stuff that belongs here... */
}

}
