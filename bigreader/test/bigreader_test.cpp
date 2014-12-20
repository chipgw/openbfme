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
            Log::info("Line #%04d: \"%s\"", i + 1, file->getLine());
        }
    }else{
        Log::warning("Unable to open file!");
    }

    file = big.openFile("..\\NewFile.inc", "folder\\inifile.ini");

    if(file != nullptr){
        for(int i = 0; !file->eof(); ++i){
            Log::info("Line #%04d: \"%s\"", i + 1, file->getLine());
        }
    }else{
        Log::warning("Unable to open file!");
    }

    return 0;
}
