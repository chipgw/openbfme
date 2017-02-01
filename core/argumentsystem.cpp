#include "argumentsystem.hpp"
#include "string.hpp"

namespace OpenBFME {

StringArgument::StringArgument(const std::initializer_list<string>& n, const string& d, bool expectValue) :
    description(d), valid(false), names(n), expectsValue(expectValue) { /* STUB */ }

void StringArgument::parse(const string& usedName){
    /* All we care about for string arguments is that it isn't empty. */
    valid = result.size() != 0;
}

void StringArgument::printHelp() {
    string nameStr;

    for(const string& name : names)
        /* Write a single dash for single letters and two otherwise, even though it doesn't really matter. */
        nameStr += ((name.size() == 1) ? "-" : "--") + name + ", ";

    /* Replace the last comma in the string with a colon. */
    nameStr[nameStr.size() - 2] = ':';

    /* Help messages are console only, with no timestamp. */
    puts(String::format("  %-24s\t%s", nameStr, description).c_str());
}

bool StringArgument::containsName(const string& name) const {
    return names.count(name) > 0;
}

void BoolArgument::parse(const string& usedName) {
    /* No argument passed = true. */
    if (result.size() == 0 || String::caseInsensitiveEquals(result, "yes") || result == "1") {
        boolResult = true;
        valid = true;
    } else if (String::caseInsensitiveEquals(result, "no") || result == "0") {
        boolResult = false;
        valid = true;
    } else {
        errorMessage = String::format("Invalid command-line! expected \"yes\", \"no\", \"1\", or \"0\", after %s, got \"%s\"", usedName, result);
    }
}

void IntegerArgument::parse(const string& usedName) {
    try {
        intResult = std::stoi(result);
        valid = true;
    } catch(...) {
        errorMessage = String::format("Invalid command-line! expected an integer value after %s, got \"%s\"", usedName, result);
    }
}

void DecimalArgument::parse(const string& usedName) {
    try {
        decResult = std::stof(result);
        valid = true;
    } catch(...) {
        errorMessage = String::format("Invalid command-line! expected a decimal value after %s, got \"%s\"", usedName, result);
    }
}

void MultiStringArgument::parse(const string& usedName) {
    if (!result.empty())
        results.push_back(result);

    valid = !results.empty();
}

}
