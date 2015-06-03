#pragma once

#include "types.hpp"
#include <map>

namespace OpenBFME{

class IniParser{
    std::map<string, string> macros;

    bool parseMacro(const BigEntry &file, IniObject &object);

    bool parseVariable(const BigEntry &file, IniVariable &var, const std::string &name);
    string getVariableWord(const BigEntry &file);

    bool parseBool(const BigEntry &file, IniVariable &var, const std::string &name);
    bool parseInteger(const BigEntry &file, IniVariable &var, const std::string &name, integer mult = 1);
    bool parseDecimal(const BigEntry &file, IniVariable &var, const std::string &name, decimal mult = 1.0f);
    bool parseVector(const BigEntry &file, IniVariable &var, const std::string &name, decimal mult = 1.0f);

public:
    EXPORT void parse(const BigEntry &file, IniObject &object);
};

}
