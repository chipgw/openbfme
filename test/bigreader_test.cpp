#include "application.hpp"
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

int main(int argc, const char* argv[]){
    Application app(argc, argv);

    app.parseArguments();

    BigFilesystem big;

    big.mount("test.big", true);

    testFile(big.openFile("folder/inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder\\inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder/"));
    testFile(big.openFile("..\\NewFile.inc", "folder\\inifile.ini"));
    testFile(big.openFile("..\\NewFile.inc", "folder/inifile.ini"));

    big.unmount("test.big");

    big.mount("test", true);

    testFile(big.openFile("folder/inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder\\inifile.ini"));
    testFile(big.openFile("inifile.ini", "folder/"));
    testFile(big.openFile("..\\NewFile.inc", "folder\\inifile.ini"));
    testFile(big.openFile("..\\NewFile.inc", "folder/inifile.ini"));

    big.unmount("test");

    return 0;
}
