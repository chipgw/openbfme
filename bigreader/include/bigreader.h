#ifndef OPENBFME_BIGREADER_H
#define OPENBFME_BIGREADER_H

#include <string>
#include <cstdio>

namespace OpenBFME {

class BigEntry;
class BigArchive;
class BigFilesystem;

class BigEntry{
    fpos_t start, end;
public:
    BigEntry(BigArchive &archive, const char* filename);
};

class BigArchive{
public:
    BigArchive(const char* filename);
};

class BigFilesystem{
public:
    BigFilesystem();

    bool mount(const char* filename, bool append);
};

}

#endif //OPENBFME_BIGREADER_H
