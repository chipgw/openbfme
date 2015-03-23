#include "application.hpp"
#include "iniparser.hpp"
#include "log.hpp"

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

int main(int argc, const char* argv[]){
    Application app(argc, argv);

    app.parseArguments();

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
