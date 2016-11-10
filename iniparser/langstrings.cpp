#include "langstrings.hpp"
#include "bigfilesystem.hpp"
#include "bigentry.hpp"
#include "log.hpp"
#include <map>
#include <algorithm>

namespace OpenBFME {

namespace LangStrings {

namespace {

/* TODO - Perhaps this should be split up by type... */
/* TODO - Should these be a wide/unicode string of some sort? */
std::map<string, string> strings;

}

bool loadStringFile(const string& filename) {
    auto file = BigFilesystem::openFile(filename);

    if (file == nullptr)
        return false;

    while (!file->eof()) {
        /* TODO - This may have problems with whitespace... */
        string key = file->getLine(true);

        if (key == "")
            continue;

        key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());

        string str;

        /* Find a word that isn't blank or a newline (just one char) and assume it's the string. */
        while (str.size() < 2)
            str = file->getWord();

        /* Trim the quotes. */
        str.erase(str.begin());
        str.erase(str.end()-1);

        Log::debug("Loaded localization string, key: %s, value: \"%s\"", key, str);

        strings[key] = str;

        /* Get words until we find "END". */
        while (key != "END")
            key = file->getWord();
    }

    return true;
}

/* TODO - Find a good way of not returning copies left and right... */
const string getString(string key) {
    /* Key strings have no whitespace, but they're loaded as lines so there may be some trailing... */
    key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());

    if (strings.count(key) != 0)
        return strings.at(key);

    return key;
}

}

}
