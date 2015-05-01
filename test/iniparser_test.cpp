#include "application.hpp"
#include "iniparser.hpp"
#include "log.hpp"

using namespace OpenBFME;

void dumpObject(const IniObject& object, string indentation){
    if(object.args.size() > 0){
        Log::info("%sArguments:", indentation);
        for(const string& arg : object.args){
            Log::info("%s  Argument \"%s\":", indentation, arg);
        }
    }

    if(object.variables.size() > 0){
        Log::info("%sVariables:", indentation);
        for(auto& var : object.variables){
            string value;

            switch(var.second.type){
            case IniVariable::Bool:
                value = var.second.b ? "true" : "false";
                break;
            case IniVariable::Integer:
                value = std::to_string(var.second.i);
                break;
            case IniVariable::Decimal:
            case IniVariable::Percent:
                value = std::to_string(var.second.d);
                break;
            case IniVariable::Line:
            case IniVariable::String:
                value = var.second.s;
                break;
            case IniVariable::Color:
            case IniVariable::Vector:
                value = format("%f, %f, %f, %f", var.second.v.x, var.second.v.y, var.second.v.z, var.second.v.w);
                break;
            }

            Log::info("%s  Variable: \"%s\" Value: \"%s\"", indentation, var.first, value);
        }
    }

    if(object.subObjects.size() > 0){
        Log::info("%sSubobjects:", indentation);
        for(auto& sub : object.subObjects){
            Log::info("%s  Subobject \"%s\":", indentation, sub.first);
            dumpObject(sub.second, indentation + "    ");
        }
    }
}

void runTest(IniParser& ini, BigFilesystem& big, IniType& rootType) {
    if(const BigEntry* file = big.openFile("folder/inifile.ini")){
        IniObject root(rootType);
        ini.parse(*file, root);

        dumpObject(root, "");
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
