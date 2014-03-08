#ifndef OPENBFME_INIPARSER_H
#define OPENBFME_INIPARSER_H

#include "bigreader.h"
#include <functional>
#include <map>

namespace OpenBFME{

class IniParser{
    BigFilesystem &filesystem;
    std::map<string, string> macros;

public:
    IniParser(BigFilesystem &filesys);

    void parse(const BigEntry &file, std::function<void(std::string, std::string)> set);
};

}

#endif //OPENBFME_INIPARSER_H
