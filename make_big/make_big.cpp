#include "application.hpp"
#include "argumentsystem.hpp"
#include "bigarchive.hpp"
#include "bigfilesystem.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    auto output = app.registerStringArgument({"output", "o"}, "Path to write the .big archive to.");

    app.parseArguments();

    auto args = app.getRemainingArgs();

    if (args.size() < 1) {
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    if (output->valid) {
        if (args.size() != 1) {
            Log::warning("Only the first folder passed will be used if the output argument is passed.");
        }

        auto archive = BigFilesystem::mount(args[0], true);

        if (archive == nullptr){
            /* There will already be an error message about the error, so we just quit. */
            exit(EXIT_FAILURE);
        }

        archive->writeBig(output->result);

        BigFilesystem::unmount(archive);
    } else {
        for (const string& arg : args) {
            auto archive = BigFilesystem::mount(arg, true);

            if (archive == nullptr){
                /* There will already be an error message about the error, so we just continue to the next file. */
                continue;
            }

            archive->writeBig(arg + ".big");

            BigFilesystem::unmount(archive);
        }
    }

    exit(EXIT_SUCCESS);
}
