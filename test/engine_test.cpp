#include "application.hpp"
#include "gameworld.hpp"
#include "iniobject.hpp"
#include "initype.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include "langstrings.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    BigFilesystem::mount("test.big", true);

    LangStrings::loadStringFile("data/test.str");

    IniType rootType("engine_test_root.ini");
    IniObject root(rootType);

    IniParser ini;

    auto plrTemplateFile = BigFilesystem::openFile("data/ini/playertemplate.ini");

    if (plrTemplateFile == nullptr) {
        Log::error("Unable to open playertemplate.ini!");
        return 1;
    }

    ini.parse(*plrTemplateFile, root);

    auto files = BigFilesystem::findFiles("data/ini/object/*.ini");

    for (const string& file : files) {
        Log::debug("Loading .ini found in data/ini/object/: \"%s\"", file);

        if (const BigEntry* entry = BigFilesystem::openFile(file))
            ini.parse(*entry, root);
    }

    GameWorld world(root);
    GamePlayer* player = world.createPlayer("FactionTest");

    if (player == nullptr) {
        Log::error("Unable to create player!");
        return 1;
    }

    for (integer i = 0; i < 20; ++i) {
        world.tick(0.1f);

        if (i % 4 == 0)
            world.createObject("Foo", *player);
    }

    return 0;
}
