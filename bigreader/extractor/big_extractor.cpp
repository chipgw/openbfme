#include "log.hpp"
#include "bigreader.hpp"
#include <vector>

using namespace OpenBFME;


int main(int argc, const char* argv[]){
    Log::init("big_extractor.log");

    std::vector<std::string> args(argv + 1, argv + argc);

    if(args.size() < 1){
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    BigFilesystem big;

    for(std::string &arg : args){
        BigArchive* archive = big.mount(arg, true);

        if(archive != nullptr){
            std::string path = arg;
            auto lastSlash = path.find_last_of("/\\");

            if(lastSlash != std::string::npos){
                path.erase(0, lastSlash + 1);
            }

            path.erase(path.find_last_of("."));
            path.push_back('/');

            archive->extractAll(path);

            big.unmount(archive);
        }
    }

    exit(EXIT_SUCCESS);
}
