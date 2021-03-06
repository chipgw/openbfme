#include "log.hpp"
#include "application.hpp"
#include "argumentsystem.hpp"
#include <algorithm>

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

namespace OpenBFME {

namespace {
/* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
Application* app = nullptr;
}

Application::Application(int argc, const char* argv[]) : fullArguments(argv + 1, argv + argc),
    startTime(std::chrono::duration_cast<TimePoint>(Clock::now().time_since_epoch())) {
    /* If app isn't nullptr an instance of this class has been created, which is bad. */
    if (app == nullptr) {
        app = this;

        /* Get the full path of the application from the first argument passed. */
        executablePath = fs::canonical(fs::path(argv[0])).string();
    } else {
        /* TODO - This may be cause for aborting. Just remember not to create an instance of this class anywhere but in main. */
        Log::error("A second instance of Application class was created!");
    }
}

Application::~Application() {
    /* Close log file and reset the static pointer when Application instance is destroyed. */
    if (app == this) {
        Log::info("Shutting down.");

        Log::shutdownLog();

        app = nullptr;
    }
}

void Application::parseArguments() {
    /* Default arguments that are available in all programs. */
    auto verbose =  registerBoolArgument({"verbose","v"},   "Write all messages to the log.");
    auto silent =   registerBoolArgument({"silent","s"},    "Only write \"ERROR\" level messages to the log.");
    auto help =     registerBoolArgument({"help","h"},      "Show this help message and quit.");

    for (auto i = fullArguments.begin(); i != fullArguments.end(); ++i) {
        const string& arg = *i;

        /* Any argument that starts with a '-' is handled by the parser. */
        if (arg.size() > 1 && arg[0] == '-') {
            /* It doesn't matter whether one or two dashes start the argument. */
            string::size_type start = (arg[1] == '-') ? 2 : 1;
            string::size_type equals = arg.find('=');

            string name = arg.substr(start, equals - start);

            auto iter = std::find_if(parsedArguments.begin(), parsedArguments.end(), [&](std::shared_ptr<StringArgument> def) {
                return def->containsName(name);
            });
            if (iter != parsedArguments.end()) {
                auto& def = *iter;
                /* If an '=' was found in the argument, use everything after it as the arg value. */
                if (equals != string::npos)
                    def->result = arg.substr(equals + 1);
                /* If there is no '=' but the arg expects a value, try using the next passed argument. */
                else if (def->expectsValue && (i+1) != fullArguments.end() && (i+1)->front() != '-')
                    def->result = *++i;
                /* If there was no value passed to the arg, clear any value that may have already been setbefore parsing. */
                else
                    def->result.clear();

                def->parse(name);
            }
        } else {
            /* If the argument wasn't preceded by a '-' and wasn't used by the previous arg, it goes in this list. */
            remainingArguments.push_back(arg);
        }
    }

    if (help->valid && help->boolResult) {
        /* Help messages are console only, with no timestamp.
         * TODO - make a way to customize this message. */
        puts(String::format("Usage: %s [OPTIONS]\n\nOptions:", executablePath.substr(executablePath.find_last_of("/\\") + 1)).c_str());

        for (auto arg : parsedArguments)
            arg->printHelp();

        /* TODO - Make a normal way to do this. */
        exit(EXIT_SUCCESS);
    }

    /* The log is stored in "logs/<executable name>.log". */
    fs::path logDir = executablePath;
    fs::path logName = logDir.replace_extension(string(".log")).filename();
    logDir = logDir.parent_path() / fs::path("logs");

    if (!fs::exists(logDir))
        fs::create_directories(logDir);

     /* Debug output is on by default in debug builds. */
#ifndef NDEBUG
    #define VERBOSE_DEFAULT true
#else
    #define VERBOSE_DEFAULT false
#endif

    /* We wait to init the log until now not only so that --verbose and --silent arguments,
     * but also so that when the help message(s) are printed the log file isn't even touched. */
    Log::initLog((logDir / logName).string(), verbose->valid ? verbose->boolResult : VERBOSE_DEFAULT, silent->valid ? silent->boolResult : false);

    Log::info("Starting \"%s\"", executablePath);

    for (auto& argDef : parsedArguments) {
        if (argDef->errorMessage.size() != 0)
            /* TODO - Does this warrant quiting? Probably... */
            Log::error(argDef->errorMessage);
    }
}

Application* Application::getApplication() {
    return app;
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

std::shared_ptr<const MultiStringArgument> Application::registerMultiStringArgument(const std::initializer_list<string>& names, const string& desc) {
    std::shared_ptr<MultiStringArgument> pointer(new MultiStringArgument(names, desc));
    parsedArguments.emplace_back(pointer);

    return pointer;
}

}
