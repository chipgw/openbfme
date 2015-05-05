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
    EXPORT IniType(const string& filename);
    EXPORT IniType(const string& filename, BigFilesystem &big);
    IniType(const IniObject& object);

    EXPORT bool loadFromXML(const string& filename);
    bool loadFromXML(TiXmlElement* element, const string &filename);

    EXPORT bool loadFromIni(const string& filename, BigFilesystem &big);
    bool loadFromIni(const IniObject& object);
};

}
