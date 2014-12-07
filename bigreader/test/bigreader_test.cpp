#include "bigreader.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(){
    Log::init("bigreader_test.log");
    BigFilesystem big;
    big.mount("test.big", true);

    const BigEntry* file = big.openFile("folder/inifile.ini");

    if(file != nullptr){
        for(int i = 0; !file->eof(); ++i){
            Log::info("line #%04d: \"%s\"", i, file->getLine());
        }
    }else{
        Log::warning("unable to open file!");
    }

    file = big.openFile("..\\NewFile.inc", "folder\\inifile.ini");

    if(file != nullptr){
        for(int i = 0; !file->eof(); ++i){
            Log::info("line #%04d: \"%s\"", i, file->getLine());
        }
    }else{
        Log::warning("unable to open file!");
    }

    return 0;
}
