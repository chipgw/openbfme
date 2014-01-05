#include "bigreader.h"
#include "log.h"
#include <thread>

using namespace OpenBFME;

int main(){
    Log::init();
    BigFilesystem big;
    big.mount("", true);
    return 0;
}
