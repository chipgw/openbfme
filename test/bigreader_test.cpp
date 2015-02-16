#include "bigreader.hpp"
#include "log.hpp"

using namespace OpenBFME;

bool testFile(const BigEntry* file){
    if(file == nullptr){
        Log::error("Unable to open file!");
        return false;
    }
    for(int i = 0; !file->eof(); ++i){
        Log::info("Line #%04d: \"%s\"", i + 1, file->getLine());
    }
    return true;
}

int main(){
    Log::init("bigreader_test.log");
    BigFilesystem big;
    big.mount("test.big", true);

    testFile(big.openFile("folder/inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder\\inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder/"));
    testFile(big.openFile("..\\NewFile.inc", "folder\\inifile.ini"));
    testFile(big.openFile("..\\NewFile.inc", "folder/inifile.ini"));

    return 0;
}
