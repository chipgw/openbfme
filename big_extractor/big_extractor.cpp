#include "log.hpp"
#include "application.hpp"
#include "bigreader.hpp"
#include <vector>

using namespace OpenBFME;

int main(int argc, const char* argv[]){
    Application app(argc, argv);

    app.parseArguments();

    auto args = app.getRemainingArgs();

    if(args.size() < 1){
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    BigFilesystem big;

    for(std::string &arg : args){
        BigArchive* archive = big.mount(arg, true);

        if(archive != nullptr){
            if(archive->getBackend() != BigArchive::Folder){
                std::string path = arg;
                auto lastSlash = path.find_last_of("/\\");

                if(lastSlash != std::string::npos){
                    path.erase(0, lastSlash + 1);
                }

                path.erase(path.find_last_of("."));
                path.push_back('/');

                archive->extractAll(path);
            } else{
                Log::warning("Cannot extract from a folder. Why would you want to anyway?");
            }

            big.unmount(archive);
        }
    }

    exit(EXIT_SUCCESS);
}
