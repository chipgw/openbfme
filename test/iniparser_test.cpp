#include "application.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"

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

void runTest(IniParser& ini, IniType& rootType) {
    if(const BigEntry* file = BigFilesystem::openFile("folder/inifile.ini")){
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

    /* For the "iniparser_test_root.ini" file. */
    BigArchive* currentDir = BigFilesystem::mount("./", true);

    IniType rootType("iniparser_test_root.ini");

    /* Don't need it anymore. */
    BigFilesystem::unmount(currentDir);

    IniParser ini;

    /* Parse from a .big file. */
    BigFilesystem::mount("test.big", true);

    Log::info("Testing from .big archive.");
    runTest(ini, rootType);

    BigFilesystem::unmount("test.big");

    /* Parse from a folder. */
    BigFilesystem::mount("test", true);

    Log::info("Testing from folder.");
    runTest(ini, rootType);

    BigFilesystem::unmount("test");

    return 0;
}
