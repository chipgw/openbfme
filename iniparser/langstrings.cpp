#include "langstrings.hpp"
#include "bigfilesystem.hpp"
#include "bigentry.hpp"
#include "log.hpp"
#include <map>

namespace OpenBFME {

namespace LangStrings {

namespace {

/* TODO - Perhaps this should be split up by type... */
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

        string str;

        /* Find a word that isn't a newline and assume it's the string. */
        while (str.size() == 0 || str == "\n")
            str = file->getWord();

        Log::debug("Loaded localizion string, key: %s, value: %s", key, str);

        strings[key] = str;

        /* Get words until we find "END". */
        while (key != "END")
            key = file->getWord();
    }

    return true;
}

const string getString(const string& key) {
    if (strings.count(key) != 0)
        return strings.at(key);

    return key;
}

}

}
