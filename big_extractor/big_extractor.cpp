#include "log.hpp"
#include "application.hpp"
#include "argumentsystem.hpp"
#include "bigarchive.hpp"
#include "bigfilesystem.hpp"

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

using namespace OpenBFME;

int main(int argc, const char* argv[]){
    Application app(argc, argv);

    auto overwrite  = app.registerBoolArgument({"overwrite","o"},       "Overwrite files if they exsist.");
    auto ignore     = app.registerBoolArgument({"ignore-existing","i"}, "Skip files if they exsist.");
    auto useSubdirs = app.registerBoolArgument({"subdirectories","d"},  "Put extracted files in subdirectories based on archive filename.");
    auto outPath    = app.registerStringArgument({"path"},              "The path to save files to.");

    app.parseArguments();

    auto args = app.getRemainingArgs();

    if(args.size() < 1){
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    fs::path basePath;

    if(outPath->valid){
        basePath = outPath->result;

        try{
            if(!fs::exists(basePath))
                fs::create_directories(basePath);
        }catch(...){
            Log::error("Could not set output path to \"%s\"", basePath.generic_string());
            exit(EXIT_FAILURE);
        }
    }


    for(std::string &arg : args){
        BigArchive* archive = BigFilesystem::mount(arg, true);

        if(archive != nullptr){
            if(archive->getBackend() != BigArchive::Folder){
                fs::path path = basePath.empty() ? fs::canonical(fs::path(arg)).parent_path() : basePath;

                if(useSubdirs->valid && useSubdirs->boolResult){
                    path /= fs::path(arg).stem();
                }
                path /= "/";

                Log::debug("Extraction path for \"%s\": \"%s\"", arg, path.generic_string());

                archive->extractAll(path.generic_string(), ignore->valid && ignore->boolResult, overwrite->valid && overwrite->boolResult);
            }else{
                Log::warning("Cannot extract from a folder. Why would you want to anyway?");
            }

            BigFilesystem::unmount(archive);
        }
    }

    exit(EXIT_SUCCESS);
}
