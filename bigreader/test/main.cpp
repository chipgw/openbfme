#include "bigreader.h"
#include "log.h"

using namespace OpenBFME;

int main(){
    Log::init();
    BigFilesystem big;
    big.mount("test.big", true);

    BigEntry* file = big.openFile("folder\\inifile.ini");

    if(file != nullptr){
        for(int i = 0; !file->eof(); ++i){
            Log::info("line #%04d: \"%s\"", i, file->getLine().c_str());
        }
    }else{
        Log::warning("unable to open file!");
    }

    return 0;
}
