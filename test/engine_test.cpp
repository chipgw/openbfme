#include "application.hpp"
#include "world.hpp"
#include "iniobject.hpp"
#include "initype.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    /* For the "iniparser_test_root.ini" file. */
    BigArchive* currentDir = BigFilesystem::mount("./", true);

    /* TODO - Make a seperate IniType definition file. */
    IniObject root(IniType("iniparser_test_root.ini"));

    /* TODO - Parse objects. */

    GameWorld world(root);

    for (integer i = 0; i < 20; ++i) {
        world.tick(0.1f);
        world.createObject("foo");
    }

    return 0;
}
