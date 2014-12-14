#ifndef OPENBFME_INIPARSER_HPP
#define OPENBFME_INIPARSER_HPP

#include "bigreader.hpp"
#include <memory>
#include <map>
#include <vector>

namespace OpenBFME{

struct IniVariable{
    enum VariableType{
        Bool,   /* Put a true/false value in b. */
        Integer,/* Put an integer in i. */
        Decimal,/* Put a decimal number in d. */
        Percent,/* Map XXX% to X.XX and put it in d */
        String, /* Put a single string value in s. */
        Color,  /* Put a RGB(A) color value in v. */
        Vector, /* Put an XYZ vector in v. */
        Line    /* Put the rest of the line in s, use when built in parsing doesn't do what you need. */
    };

    VariableType type;

    union{
        bool b;
        integer i;
        decimal d;
    };
    vec4 v;
    string s;
};

struct IniType{
    std::map<string, IniVariable::VariableType> variableTypes;
    std::map<string, IniType> subTypes;

    bool breaks = true;
    string breakWord = "End";
};

struct IniObject{
    const IniType &type;
    std::vector<string> args;

    std::map<string, IniObject> subObjects;
    std::map<string, IniVariable> variables;

    IniObject(const IniType &baseType) : type(baseType) { }
};

class IniParser{
    BigFilesystem &filesystem;
    std::map<string, string> macros;

public:
    EXPORT IniParser(BigFilesystem &filesys);

    EXPORT void parse(const BigEntry &file, IniObject &object);

private:
    bool parseMacro(const BigEntry &file, IniObject &object);

    bool parseVariable(const BigEntry &file, IniVariable &var, const std::string &name);

    bool parseBool(const BigEntry &file, IniVariable &var, const std::string &name);
    bool parseInteger(const BigEntry &file, IniVariable &var, const std::string &name, integer mult = 1);
    bool parseDecimal(const BigEntry &file, IniVariable &var, const std::string &name, decimal mult = 1.0f);
    bool parseVector(const BigEntry &file, IniVariable &var, const std::string &name, decimal mult = 1.0f);
};

}

#endif //OPENBFME_INIPARSER_HPP
