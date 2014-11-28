#ifndef OPENBFME_INIPARSER_HPP
#define OPENBFME_INIPARSER_HPP

#include "bigreader.hpp"
#include <memory>
#include <map>
#include <vector>

namespace OpenBFME{


struct IniVariable{
    enum VariableType{
        Integer,/* for integer numbers and bitflags */
        Decimal,/* for decimal numbers */
        String  /* for a single string value */
    };

    VariableType type;

    union{
        integer i;
        decimal d;
    };
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

    EXPORT void parse(const BigEntry &file, IniObject object);
};

}

#endif //OPENBFME_INIPARSER_HPP
