#include "iniparser.hpp"
#include "log.hpp"
#include <memory>

using namespace OpenBFME;

void runTest(IniParser& ini, BigFilesystem& big, IniType& rootType) {
    if(const BigEntry* file = big.openFile("folder/inifile.ini")){
        IniObject root(rootType);
        ini.parse(*file, root);

        /* TODO - check loaded data. */
    }else{
        Log::warning("Unable to open file!");
    }
}

int main(){
    Log::init("iniparser_test.log");

    IniType rootType("iniparser_test_root.xml");

    BigFilesystem big;
    IniParser ini(big);

    /* Parse from a .big file. */
    big.mount("test.big", true);

    runTest(ini, big, rootType);

    big.unmount("test.big");

    /* Parse from a folder. */
    big.mount("test", true);

    runTest(ini, big, rootType);

    big.unmount("test");

    return 0;
}
