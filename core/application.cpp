#include "application.hpp"
#include "log.hpp"
#include "argumentsystem.hpp"
#include <algorithm>

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

namespace OpenBFME {

Application::Application(int argc, const char *argv[]){
    if(app == nullptr){
        app = this;
        fullArguments = std::vector<string>(argv + 1, argv + argc);

        executablePath = fs::canonical(fs::path(argv[0])).string();
    }else{
        /* TODO - This may be cause for aborting. Just remember not to create an instance of this class anywhere but in main. */
        Log::error("A second instance of Application class was created!");
    }
}

Application::~Application(){
    /* Close log file and reset the static pointer when Application instance is destroyed. */
    if(app == this){
        Log::info("Shutting down.");
        Log::shutdown();
        app = nullptr;
    }
}

void Application::parseArguments(){
    auto verbose = registerArgument<BoolArgument>({"verbose","v"}, "TODO - document.");
    auto silent = registerArgument<BoolArgument>({"silent","s"}, "TODO - document.");

    for(string& arg : fullArguments){
        /* Any argument that starts with a '-' is handled by the parser. */
        if(arg.size() > 1 && arg[0] == '-'){
            /* It doesn't matter whether one or two dashes start the argument. */
            string::size_type start = (arg[1] == '-') ? 2 : 1;
            string::size_type equals = arg.find('=');

            string key = arg.substr(start, equals - start);

            auto iter = std::find_if(parsedArguments.begin(), parsedArguments.end(), [&](std::shared_ptr<StringArgument> def){
                return def->containsName(key);
            });
            if(iter != parsedArguments.end()){
                if(equals != string::npos)
                    (*iter)->result = arg.substr(equals + 1);
                else
                    (*iter)->result.clear();

                (*iter)->parse();
            }
        }else{
            remainingArguments.push_back(arg);
        }
    }

    /* The log is stored in "logs/<executable name>.log". */
    fs::path logDir = executablePath;
    fs::path logName = fs::change_extension(logDir, string(".log")).filename();
    logDir = logDir.parent_path() / fs::path("logs");

    if(!fs::exists(logDir))
        fs::create_directories(logDir);

    Log::init((logDir / logName).string(), verbose->valid ? verbose->boolResult : false, silent->valid ? silent->boolResult : false);

    Log::info("Starting \"%s\"", executablePath);
}

Application* Application::app = nullptr;

}
