#pragma once

#include "inivariable.hpp"
#include <map>

namespace OpenBFME{

class IniType {
public:
    std::map<string, IniVariable::VariableType> variableTypes;
    std::map<string, IniType> subTypes;

    /* Whether or not we stop parsing when we reach the breakWord. */
    bool breaks = true;
    string breakWord = "End";

    IniType() = default;

    /* Load an IniType from an .ini file. */
    EXPORT IniType(const string& filename);

    /* Load an IniType from an IniObject. */
    IniType(const IniObject& object);

    /* Load an IniType from an .ini file. */
    EXPORT bool loadFromIni(const string& filename);

    /* Load an IniType from an IniObject. */
    bool loadFromIni(const IniObject& object);
};

}
