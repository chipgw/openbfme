#include "application.hpp"
#include "log.hpp"
#include <algorithm>

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

/* These name differences should be fixed in VS 2015. */
#ifdef OPENBFME_COMPILER_MSVC
#define canonical complete
#define change_extension replace_extension
#endif

namespace OpenBFME {

Application::Application(int argc, const char *argv[]){
    if(app == nullptr){
        app = this;
        fullArguments = std::vector<string>(argv + 1, argv + argc);

        executablePath = fs::canonical(fs::path(argv[0])).string();

        bool verbose = false, silent = false;

        for(string& arg : fullArguments){
            /* TODO - Make a whole fancy system for handling other options. */
            if(arg == "-v" || arg == "--verbose")
                verbose = true;
            else if(arg == "-s" || arg == "--silent")
                silent = true;
            else
                remainingArguments.push_back(arg);
        }

        fs::path logPath = executablePath;
        fs::path logName = fs::change_extension(logPath, string(".log")).filename();
        logPath = logPath.parent_path() / fs::path("logs");

        if(!fs::exists(logPath))
            fs::create_directories(logPath);

        Log::init((logPath / logName).string(), verbose, silent);

        Log::info("Starting \"%s\"", executablePath);
    }else{
        /* TODO - This may be cause for aborting. Just remember not to create an instance of this class anywhere but in main. */
        Log::error("A second instance of Application class was created!");
    }
}

Application* Application::app = nullptr;

}
