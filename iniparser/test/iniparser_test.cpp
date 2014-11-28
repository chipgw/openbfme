#include "iniparser.hpp"
#include "log.hpp"
#include <memory>

using namespace OpenBFME;

int main(){
    Log::init("iniparser_test.log");
    BigFilesystem big;
    IniParser ini(big);
    big.mount("test.big", true);

    const BigEntry* file = big.openFile("folder/inifile.ini");

    IniType rootType;
    rootType.subTypes["Object"].variableTypes["Variable"] = IniVariable::Integer;

    IniObject root(rootType);

    if(file != nullptr){
        ini.parse(*file, root);
    }else{
        Log::warning("unable to open file!");
    }
    return 0;
}
