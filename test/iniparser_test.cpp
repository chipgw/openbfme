#include "log.hpp"
#include "application.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include "langstrings.hpp"

using namespace OpenBFME;

/* Print a bunch of information about the object, recursively printing subobjects with a higher indentation. */
void dumpObject(const IniObject& object, string indentation = "") {
    if (object.args.size() > 0) {
        Log::info("%sArguments:", indentation);
        for (const string& arg : object.args)
            Log::info("%s  \"%s\"", indentation, arg);
    }

    if (object.variables.size() > 0){
        Log::info("%sVariables:", indentation);
        for (auto& var : object.variables) {
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
                value = '"' + var.second.s + '"';
                break;
            case IniVariable::Color:
            case IniVariable::Vector:
                value = String::format("%f, %f, %f, %f", var.second.v.x, var.second.v.y, var.second.v.z, var.second.v.w);
                break;
            }

            Log::info("%s  Variable: \"%s\" Value: %s", indentation, var.first, value);
        }
    }

    if (object.subObjects.size() > 0) {
        Log::info("%sSubobjects:", indentation);
        for (auto& sub : object.subObjects) {
            Log::info("%s  Subobject \"%s\":", indentation, sub.first);
            dumpObject(sub.second, indentation + "    ");
        }
    }
}

void runTest(IniParser& ini, IniType& rootType) {
    if (const BigEntry* file = BigFilesystem::openFile("folder/inifile.ini")) {
        IniObject root(rootType);
        ini.parse(*file, root);

        dumpObject(root);
    } else {
        Log::warning("Unable to open file!");
    }
}

void testStrings() {
    Log::info("Loading string file \"data/test.str\"...");

    LangStrings::loadStringFile("data/test.str");

    Log::info("OBJECT:TestFortress value: %s", LangStrings::getString("OBJECT:TestFortress"));
    Log::info("OBJECT:FortressDescription value: %s", LangStrings::getString("OBJECT:FortressDescription"));

    Log::info("Done loading strings.");
}

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    IniParser ini;

    /* Parse from a .big file. */
    BigFilesystem::mount("test.big", true);

    /* This is now in the test.big archive. */
    IniType rootType("iniparser_test_root.ini");

    Log::info("Testing from .big archive.");
    runTest(ini, rootType);

    testStrings();

    BigFilesystem::unmount("test.big");

    /* Parse from a folder. */
    BigFilesystem::mount("test", true);

    Log::info("Testing from folder.");
    runTest(ini, rootType);

    testStrings();

    BigFilesystem::unmount("test");

    return 0;
}
