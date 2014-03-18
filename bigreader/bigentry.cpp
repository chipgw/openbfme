#include "bigreader.hpp"
#include "log.hpp"
#include <cstdio>

namespace OpenBFME {

BigEntry::BigEntry(BigArchive &arch, uint32_t start, uint32_t end, string file) : archive(arch), start(start), end(end), filename(file) {}

}
