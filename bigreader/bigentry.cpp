#include "bigreader.h"
#include "log.h"
#include <cstdio>

namespace OpenBFME {

BigEntry::BigEntry(BigArchive &arch, uint32_t start, uint32_t end, std::string file) : archive(arch), start(start), end(end), filename(file) {}

}
