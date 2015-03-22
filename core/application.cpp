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

        executablePath = fs::canonical(fs::path(argv[0])).string();

        for(string& arg : fullArguments){
            /* Any argument that starts with a '-' is handled by the parser. */
            if(arg.size() > 1 && arg[0] == '-'){
                /* It doesn't matter whether one or two dashes start the argument. */
                string::size_type start = (arg[1] == '-') ? 2 : 1;
                string::size_type equals = arg.find('=');

                string key = arg.substr(start, equals - start);

                if(equals != string::npos)
                    parsedArguments[key] = arg.substr(equals + 1);
                else
                    parsedArguments[key] = "";
            }else{
                remainingArguments.push_back(arg);
            }
        }

        /* The log is stored in "logs/<executable name>.log". */
        fs::path logPath = executablePath;
        fs::path logName = fs::change_extension(logPath, string(".log")).filename();
        logPath = logPath.parent_path() / fs::path("logs");

        if(!fs::exists(logPath))
            fs::create_directories(logPath);

        Log::init((logPath / logName).string(), getBoolArgument("verbose|v"), getBoolArgument("silent|s"));

        Log::info("Starting \"%s\"", executablePath);
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

bool Application::getArgumentName(string &name){
    string::size_type delim = 0;

    while((delim = name.find('|')) != string::npos){
        if(parsedArguments.count(name.substr(0, delim)) != 0)
            name.erase(delim);
        else
            name.erase(0, delim + 1);
    }

    if(parsedArguments.count(name) == 0)
        name.clear();

    return name.size() != 0;
}

bool Application::getBoolArgument(string name, bool *valid) {
    if(valid) *valid = true;

    if(getArgumentName(name)){
        const string& arg = parsedArguments[name];

        if(arg.size() == 0 || arg == "yes" || arg == "1")
            return true;

        if(arg == "no" || arg == "0")
            return false;

        Log::error("Invalid command-line: Expected \"yes\", \"no\", \"1\", \"0\" after \"--%s\", got \"%s\"!", name, arg);
    }

    if(valid) *valid = false;
    return false;
}

integer Application::getIntegerArgument(string name, bool *valid){
    if(valid) *valid = true;

    if(getArgumentName(name)){
        const string& arg = parsedArguments[name];
        try{
            return std::stoi(arg);
        }catch(...){
            Log::error("Invalid command-line: Expected integer value after \"--%s\", got \"%s\"!", name, arg);
        }
    }

    if(valid) *valid = false;
    return 0;
}

decimal Application::getDecimalArgument(string name, bool *valid){
    if(valid) *valid = true;

    if(getArgumentName(name)){
        const string& arg = parsedArguments[name];
        try{
            return std::stof(arg);
        }catch(...){
            Log::error("Invalid command-line: Expected decimal value after \"--%s\", got \"%s\"!", name, arg);
        }
    }

    if(valid) *valid = false;
    return 0.0f;
}

string Application::getStringArgument(string name, bool *valid){
    if(valid) *valid = true;

    if(getArgumentName(name))
        return parsedArguments[name];

    if(valid) *valid = false;
    return "";
}

Application* Application::app = nullptr;

}
