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

        for(string& arg : fullArguments){
            if(arg.size() > 1 && arg[0] == '-'){
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

        fs::path logPath = executablePath;
        fs::path logName = fs::change_extension(logPath, string(".log")).filename();
        logPath = logPath.parent_path() / fs::path("logs");

        if(!fs::exists(logPath))
            fs::create_directories(logPath);

        Log::init((logPath / logName).string(), getBoolArgument("verbose") || getBoolArgument("v"), getBoolArgument("silent") || getBoolArgument("s"));

        Log::info("Starting \"%s\"", executablePath);
    }else{
        /* TODO - This may be cause for aborting. Just remember not to create an instance of this class anywhere but in main. */
        Log::error("A second instance of Application class was created!");
    }
}

bool Application::getBoolArgument(string name, bool *valid) {
    if(valid) *valid = false;

    bool stringValid;
    string arg = getStringArgument(name, &stringValid);

    if(!stringValid)
        return false;

    if(arg.size() == 0 || arg == "yes" || arg == "1"){
        if(valid) *valid = true;
        return true;
    }
    if(arg == "no" || arg == "0"){
        if(valid) *valid = true;
        return false;
    }

    Log::error("Invalid command-line: Expected \"yes\", \"no\", \"1\", \"0\" after \"--%s\", got \"%s\"!", name, arg);
    return false;
}

integer Application::getIntegerArgument(string name, bool *valid){
    string arg = getStringArgument(name, valid);
    try{
        return std::stoi(arg);
    }catch(...){
        Log::error("Invalid command-line: Expected integer value after \"--%s\", got \"%s\"!", name, arg);
    }

    if(valid) *valid = false;
    return 0;
}

decimal Application::getDecimalArgument(string name, bool *valid){
    string arg = getStringArgument(name, valid);

    try{
        return std::stof(arg);
    }catch(...){
        Log::error("Invalid command-line: Expected decimal value after \"--%s\", got \"%s\"!", name, arg);
    }

    if(valid) *valid = false;
    return 0.0f;
}

string Application::getStringArgument(string name, bool *valid){
    if(parsedArguments.count(name) == 0){
        if(valid) *valid = false;
        return "";
    }

    if(valid) *valid = true;

    return parsedArguments[name];
}

Application* Application::app = nullptr;

}
