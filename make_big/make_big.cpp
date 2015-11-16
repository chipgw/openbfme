#include "log.hpp"
#include "application.hpp"
#include "argumentsystem.hpp"
#include "bigarchive.hpp"
#include "bigfilesystem.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    /* Arguments are used in the same order as they were passed. i.e. The 1st & 2nd input args use the 1st & 2nd output arg respectively. */
    auto output = app.registerMultiStringArgument({"output", "o"}, "Path to write the .big archive to.");

    app.parseArguments();

    auto args = app.getRemainingArgs();

    if (args.size() == 0) {
        Log::error("Not enough arguments supplied!");
        exit(EXIT_FAILURE);
    }

    /* If any output args are passed, warn if there aren't the same amount of folder args. */
    if (output->valid && args.size() != output->results.size())
        Log::warning("Wrong number of output arguments passed. Expected: %d, got %d.", args.size(), output->results.size());

    for (size_t i = 0; i < args.size(); ++i) {
        auto archive = BigFilesystem::mount(args[i], true);

        /* There will already be an error message about the error, so we just continue to the next file. */
        if (archive == nullptr) continue;

        /* If an output was specified, use it. Otherwise just add ".big" to the input path. */
        archive->writeBig((i < output->results.size()) ? (output->results[i]) : (args[i] + ".big"));

        BigFilesystem::unmount(archive);
    }

    exit(EXIT_SUCCESS);
}
