#include "log.h"
#include "bigreader.h"

using namespace OpenBFME;


int main(int argv, const char* argc[]){
    Log::init();

    if(argv < 2){
        Log::error("");
    }

    BigFilesystem big;

    for(int a = 1; a < argv; ++a){
        BigArchive* archive = big.mount(argc[a], true);

        if(archive != nullptr){
            // TODO - choose a better path to extract to.
            archive->extractAll("out/");

            big.unmount(archive);
        }
    }
}
