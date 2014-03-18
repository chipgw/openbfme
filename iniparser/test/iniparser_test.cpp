#include "iniparser.hpp"
#include "log.hpp"

using namespace OpenBFME;

void setValue(std::string name, std::string value){
    Log::info("set variable %s value %s", name.c_str(), value.c_str());
}

int main(){
    Log::init("iniparser_test.log");
    BigFilesystem big;
    IniParser ini(big);
    big.mount("test.big", true);

    const BigEntry* file = big.openFile("folder/inifile.ini");

    if(file != nullptr){
        ini.parse(*file, setValue);
    }else{
        Log::warning("unable to open file!");
    }
    return 0;
}
