#include "bigreader.h"
#include "log.h"
#include <cstdio>

namespace OpenBFME {

BigFilesystem::BigFilesystem(){
    Log::info("BigFilesystem created.");
}

bool BigFilesystem::mount(const char *filename, bool append){
    Log::info("Attempting to mount \"%s\" in %s mode", filename, append ? "append" : "prepend");
}

}
