#pragma once

#include "types.hpp"
#include "inivariable.hpp"
#include <map>

class TiXmlElement;

namespace OpenBFME{

class IniType{
public:
    std::map<string, IniVariable::VariableType> variableTypes;
    std::map<string, IniType> subTypes;

    bool breaks = true;
    string breakWord = "End";

    IniType() = default;
    IniType(const string& filename) { loadFromXML(filename); }

    EXPORT bool loadFromXML(const string& filename);
    bool loadFromXML(TiXmlElement* element, const string &filename);
};

}
