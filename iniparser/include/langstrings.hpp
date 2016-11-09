#pragma once

#include "types.hpp"

namespace OpenBFME {

namespace LangStrings {

/* Load localized strings from a .str file. */
EXPORT bool loadStringFile(const string& filename);

/* Get a localized string from the specified key, returns the key when the string wasn't found. */
EXPORT const string getString(const string& key);

}

}
