#include "gameobject.hpp"
#include "gameplayer.hpp"
#include "iniobject.hpp"
#include "langstrings.hpp"
#include "log.hpp"

namespace OpenBFME {

GameObject::GameObject(GamePlayer& owner, const IniObject& objTemplate, const string& objName)
    : world(owner.world), player(owner), objectTemplate(objTemplate), name(objName) {
    /* Get the display name string key from INI. */
    auto displayNameKey = objectTemplate.variables.find("DisplayName");

    /* TODO - What happens if it doesn't exist? Just having an empty string probably isn't the best way to handle it... */
    if (displayNameKey != objectTemplate.variables.end())
        /* Get the localized string with the specified key. */
        Log::debug("Object display name: \"%s\"",  displayName = LangStrings::getString(displayNameKey->second.s));

    /* Get the description string key from INI. */
    auto descriptionKey = objectTemplate.variables.find("Description");

    /* TODO - What happens if it doesn't exist? Just having an empty string probably isn't the best way to handle it... */
    if (descriptionKey != objectTemplate.variables.end())
        /* Get the localized string with the specified key. */
        Log::debug("Object description: \"%s\"",  description = LangStrings::getString(descriptionKey->second.s));

    /* TODO - init stuff. */
}

void GameObject::tick(decimal delta) {
    Log::debug("Object ticked. Object Name: \"%s\" Delta: %f", name, delta);

    /* TODO - There is a heck of a lot of stuff that belongs here... */
}

}
