#pragma once

#include "types.hpp"

namespace OpenBFME {

namespace BigFilesystem {

EXPORT BigArchive* mount(const string &filename, bool append);
EXPORT bool unmount(const string &filename);
EXPORT bool unmount(BigArchive* archive);

EXPORT const BigEntry *openFile(const string &filename, const string &relativeTo = "");

}

}
