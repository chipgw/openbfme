#include "application.hpp"
#include "log.hpp"
#include <algorithm>

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

namespace OpenBFME {

Application::Application(int argc, const char *argv[]){
    if(app == nullptr){
        app = this;
        fullArguments = std::vector<string>(argv + 1, argv + argc);

        executablePath = fs::canonical(argv[0]).string();

        bool verbose, silent;

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
        fs::path logName = fs::change_extension(logPath.filename(), ".log");
        logPath = logPath.parent_path() / "logs";

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
