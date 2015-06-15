#include "application.hpp"
#include "world.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    GameWorld world;

    for (integer i = 0; i < 20; ++i) {
        world.tick(0.1f);
        world.createObject();
    }

    return 0;
}
