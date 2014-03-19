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

    IniType root;
    root.setRawVariable = [] (const string &name, const IniType::StringArgs &args) {
        Log::info("root set variable %s", name.c_str());
    };

    root.subTypes["Object"] = [](const IniType::StringArgs &args){
        IniType type;
        std::shared_ptr<string> typeName(new string("Object"));

        if(args.size() == 1){
            /* TODO - error on wrong number of args. */
            *typeName = args[0];
            Log::info("created object: %s", typeName->c_str());

            /* NOTE: You must be very careful about capturing variables.
             * The function is called from a completely different scope so they may not still be around!
             * This function captures a std::shared_ptr, and does so by copy, so that the capture will keep the object alive.
             * For safety's sake don't do auto capture. */
            type.setRawVariable = [typeName] (string name, IniType::StringArgs args) {
                Log::info("%s set variable %s", typeName->c_str(), name.c_str());
            };
        }

        return type;
    };

    if(file != nullptr){
        ini.parse(*file, root);
    }else{
        Log::warning("unable to open file!");
    }
    return 0;
}
