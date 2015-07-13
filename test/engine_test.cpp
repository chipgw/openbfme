#include "application.hpp"
#include "gameworld.hpp"
#include "iniobject.hpp"
#include "initype.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    BigFilesystem::mount("test.big", true);

    IniType rootType("engine_test_root.ini");
    IniObject root(rootType);

    IniParser ini;

    auto files = BigFilesystem::findFiles("data/ini/objects/*.ini");

    for (const string& file : files) {
        Log::debug("Loading .ini found in data/ini/objects/: \"%s\"", file);

        if (const BigEntry* entry = BigFilesystem::openFile(file))
            ini.parse(*entry, root);
    }

    GameWorld world(root);

    for (integer i = 0; i < 20; ++i) {
        world.tick(0.1f);

        if (i % 2 == 0)
            world.createObject("Foo");
    }

    return 0;
}
