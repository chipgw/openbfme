#include "log.h"
#include "bigreader.h"
#include <vector>

using namespace OpenBFME;


int main(int argc, const char* argv[]){
    Log::init();

    std::vector<std::string> args(argv + 1, argv + argc);

    if(args.size() < 1){
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    BigFilesystem big;

    for(std::string &arg : args){
        BigArchive* archive = big.mount(arg, true);

        if(archive != nullptr){
            // TODO - choose a better path to extract to.
            archive->extractAll("out/");

            big.unmount(archive);
        }
    }

    exit(EXIT_SUCCESS);
}
