#pragma once

#include "types.hpp"
#include "inivariable.hpp"
#include <map>
#include <vector>

namespace OpenBFME {

struct IniObject {
    const IniType &type;
    std::vector<string> args;

    std::multimap<string, IniObject> subObjects;
    std::multimap<string, IniVariable> variables;

    IniObject(const IniType &baseType) : type(baseType) { }
};

}
