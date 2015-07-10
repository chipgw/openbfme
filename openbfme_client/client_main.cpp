#include "log.hpp"
#include "application.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);
    app.parseArguments();

    Log::error("OpenBFME client not yet implemented!");

    return 0;
}
