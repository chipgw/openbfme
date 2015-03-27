#pragma once

#include "types.hpp"
#include <unordered_set>

namespace OpenBFME {

class StringArgument{
private:
    const std::unordered_set<string> names;

    /* Description for the argument for use in --help. */
    const string description;

public:
    /* Whether or not this argument type needs a value passed afterward. Defaults to true. */
    const bool expectsValue;

    /* Is the parsed value valid? */
    bool valid;

    /* The raw string value passed. */
    string result;

    /* If there was an error parsing, put a message describing it here. */
    string errorMessage;

    /* Take the result string and save the value in the subclass specific variable. */
    virtual void parse(const string& usedName);

    void printHelp();

    /* check to se if the specified name exists in names. */
    bool containsName(const string& name) const;

    /* Create from an initializer_list of names and a description string. */
    StringArgument(const std::initializer_list<string>& n, const string& d, bool expectValue = true);

    /* No copying thank you very much. */
    StringArgument(const StringArgument&) = delete;
    StringArgument& operator=(const StringArgument&) = delete;
};

class BoolArgument : public StringArgument {
public:
    bool boolResult;
    virtual void parse(const string& usedName);

    BoolArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d, false) {}
};

class IntegerArgument : public StringArgument {
public:
    integer intResult;
    virtual void parse(const string& usedName);

    IntegerArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d, true) {}
};

class DecimalArgument : public StringArgument {
public:
    decimal decResult;
    virtual void parse(const string& usedName);

    DecimalArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d, true) {}
};

}
