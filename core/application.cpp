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
    auto verbose =  registerBoolArgument({"verbose","v"},   "Write all messages to the log.");
    auto silent =   registerBoolArgument({"silent","s"},    "Only write \"ERROR\" level messages to the log.");
    auto help =     registerBoolArgument({"help","h"},      "Show this help message and quit.");

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

    if(help->valid && help->boolResult){
        /* Help messages are console only, with no timestamp.
         * TODO - make a way to customize this message. */
        puts(format("Usage: %s [OPTIONS]\n\nOptions:", executablePath.substr(executablePath.find_last_of("/\\") + 1)).c_str());

        for(auto argdef : parsedArguments){
            argdef->printHelp();
        }

        /* TODO - Make a normal way to do this. */
        exit(EXIT_SUCCESS);
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

std::shared_ptr<const BoolArgument> Application::registerBoolArgument(const std::initializer_list<string>& names, const string& desc) {
    std::shared_ptr<BoolArgument> pointer(new BoolArgument(names, desc));
    parsedArguments.emplace_back(pointer);

    return pointer;
}

std::shared_ptr<const IntegerArgument> Application::registerIntegerArgument(const std::initializer_list<string>& names, const string& desc) {
    std::shared_ptr<IntegerArgument> pointer(new IntegerArgument(names, desc));
    parsedArguments.emplace_back(pointer);

    return pointer;
}

std::shared_ptr<const DecimalArgument> Application::registerDecimalArgument(const std::initializer_list<string>& names, const string& desc) {
    std::shared_ptr<DecimalArgument> pointer(new DecimalArgument(names, desc));
    parsedArguments.emplace_back(pointer);

    return pointer;
}

std::shared_ptr<const StringArgument> Application::registerStringArgument(const std::initializer_list<string>& names, const string& desc) {
    std::shared_ptr<StringArgument> pointer(new StringArgument(names, desc));
    parsedArguments.emplace_back(pointer);

    return pointer;
}


Application* Application::app = nullptr;

}
