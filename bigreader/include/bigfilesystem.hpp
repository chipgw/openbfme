#pragma once

#include "types.hpp"
#include <set>

namespace OpenBFME {

namespace BigFilesystem {

EXPORT BigArchive* mount(const string& filename, bool append);
EXPORT bool unmount(const string& filename);
EXPORT bool unmount(BigArchive* archive);

EXPORT const BigEntry* openFile(const string& filename, const string& relativeTo = "");

/* Find all files matching string with wildcard or regular expression. */
EXPORT std::set<string> findFiles(string str, bool useRegex = false);

}

}
