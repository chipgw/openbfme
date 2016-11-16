#pragma once

#include "types.hpp"
#include "inivariable.hpp"
#include <map>
#include <vector>

namespace OpenBFME {

struct IniObject {
    const IniType& type;
    std::vector<string> args;

    std::multimap<string, IniObject> subObjects;
    std::multimap<string, IniVariable> variables;

    IniObject(const IniType& baseType) : type(baseType) { }

    /* Get the first variable found with the specified key. */
    inline bool getBooleanVariable(const string& key, bool defaultVal = false) const {
        auto iter = variables.find(key);

        /* TODO - What happens if it doesn't exist? */
        if (iter != variables.end())
            return iter->second.b;

        return defaultVal;
    }
    /* Get the first variable found with the specified key. */
    inline integer getIntegerVariable(const string& key, integer defaultVal = -1) const {
        auto iter = variables.find(key);

        /* TODO - What happens if it doesn't exist? */
        if (iter != variables.end())
            return iter->second.i;

        return defaultVal;
    }
    /* Get the first variable found with the specified key. */
    inline decimal getDecimalVariable(const string& key, decimal defaultVal = -1.0f) const {
        auto iter = variables.find(key);

        /* TODO - What happens if it doesn't exist? */
        if (iter != variables.end())
            return iter->second.d;

        return defaultVal;
    }
    /* Get the first variable found with the specified key. */
    inline vec4 getVectorVariable(const string& key, vec4 defaultVal = vec4()) const {
        auto iter = variables.find(key);

        /* TODO - What happens if it doesn't exist? */
        if (iter != variables.end())
            return iter->second.v;

        return defaultVal;
    }
    /* Get the first variable found with the specified key. */
    inline string getStringVariable(const string& key, string defaultVal = "") const {
        auto iter = variables.find(key);

        /* TODO - What happens if it doesn't exist? */
        if (iter != variables.end())
            return iter->second.s;

        return defaultVal;
    }
};

}
