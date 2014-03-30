#ifndef OPENBFME_INIPARSER_HPP
#define OPENBFME_INIPARSER_HPP

#include "bigreader.hpp"
#include <functional>
#include <map>
#include <vector>

namespace OpenBFME{

struct IniType{
    enum VariableType{
        Integer,/* for integer numbers and bitflags */
        Decimal,/* for decimal numbers */
        String, /* for a single string value */
        Raw     /* call setRawVariable with the raw StringArgs */
    };

    struct Variable{
        VariableType type;
        void* variable = nullptr;
    };

    /* NOTE: You must be very careful about capturing variables in lambda functions.
     * These functions are called from a completely different scope so the variables may not still be around!
     * For safety's sake don't do auto capture. */
    typedef std::vector<string> StringArgs;
    typedef std::function<void(const string&, const StringArgs&)> RawVariableSetter;
    typedef std::function<IniType(const StringArgs&)> Creator;

    RawVariableSetter setRawVariable;

    bool breaks = true;
    string breakWord = "End";

    std::map<string, Creator> subTypes;
    std::map<string, Variable> variables;
};

class IniParser{
    BigFilesystem &filesystem;
    std::map<string, string> macros;

public:
    EXPORT IniParser(BigFilesystem &filesys);

    EXPORT void parse(const BigEntry &file, IniType type);
};

}

#endif //OPENBFME_INIPARSER_HPP
