#include "log.hpp"

using namespace OpenBFME;

int main() {
    Log::init("openbfme_server.log");

    Log::error("OpenBFME server not yet implemented!");

    return 0;
}
