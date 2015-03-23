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
    /* Is the parsed value valid? */
    bool valid;

    /* The raw string value passed. */
    string result;

    /* Take the result string and asve the value in the subclass specific variable. */
    virtual void parse();

    /* check to se if the specified name exists in names. */
    bool containsName(const string& name) const;

    /* Create from an initializer_list of names and a description string. */
    StringArgument(const std::initializer_list<string>& n, const string& d);

    /* No copying thank you very much. */
    StringArgument(const StringArgument&) = delete;
    StringArgument& operator=(const StringArgument&) = delete;
};

class BoolArgument : public StringArgument {
public:
    bool boolResult;
    virtual void parse();

    BoolArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d) {}
};

class IntegerArgument : public StringArgument {
public:
    integer intResult;
    virtual void parse();

    IntegerArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d) {}
};

class DecimalArgument : public StringArgument {
public:
    decimal decResult;
    virtual void parse();

    DecimalArgument(const std::initializer_list<string>& n, const string& d) : StringArgument(n, d) {}
};

}
