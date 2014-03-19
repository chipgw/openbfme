#include "bigreader.hpp"

namespace OpenBFME {

BigEntry::BigEntry(BigArchive &arch, uint32_t start, uint32_t end, string file) : archive(arch), start(start), end(end), filename(file) {}

}
