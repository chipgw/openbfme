#include "application.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    return 0;
}
