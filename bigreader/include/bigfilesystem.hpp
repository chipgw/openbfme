#pragma once

#include "bigarchive.hpp"
#include <list>

namespace OpenBFME {

class BigFilesystem{
    std::list<BigArchive> archives;

public:
    EXPORT BigArchive* mount(const string &filename, bool append);
    EXPORT bool unmount(const string &filename);
    EXPORT bool unmount(BigArchive* archive);

    EXPORT const BigEntry *openFile(const string &filename, const string &relativeTo = "");
};

}
