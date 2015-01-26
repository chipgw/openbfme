#include "iniparser.hpp"
#include "log.hpp"
#include <memory>

using namespace OpenBFME;

int main(){
    Log::init("iniparser_test.log");

    IniType rootType;
    rootType.loadFromXML("iniparser_test_root.xml");


    IniObject root(rootType);
    BigFilesystem big;
    IniParser ini(big);
    big.mount("test.big", true);

    const BigEntry* file1 = big.openFile("folder/inifile.ini");

    if(file1 != nullptr){
        ini.parse(*file1, root);
    }else{
        Log::warning("Unable to open file!");
    }

    big.unmount("test.big");

    big.mount("test", true);

    const BigEntry* file2 = big.openFile("folder/inifile.ini");

    if(file2 != nullptr){
        ini.parse(*file2, root);
    }else{
        Log::warning("Unable to open file!");
    }
    return 0;
}
