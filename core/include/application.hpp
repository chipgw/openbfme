#pragma once

#include "log.hpp"
#include <vector>
#include <memory>
#include <chrono>

namespace OpenBFME {

class StringArgument;
class BoolArgument;
class IntegerArgument;
class DecimalArgument;

/* This class parses commandline and initializes the Log. */
class Application{
private:
    /* Define the standard clock and time point size. */
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::nanoseconds TimePoint;

    typedef std::vector<string> ArgumentList;

    /* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
    static Application* app;

    /* All the arguments passed to the application, minus the executable path itself. */
    const ArgumentList fullArguments;

    /* Arguments handled by command line parser. */
    std::vector<std::shared_ptr<StringArgument>> parsedArguments;

    /* Any arguments not handled by the commandline parser. */
    ArgumentList remainingArguments;

    /* The path of the executable file that is running. */
    string executablePath;

    const TimePoint startTime;

    std::vector<Log::Output> logOutputs;

    void initLog(const string& filename, bool verbose, bool silent);

public:
    EXPORT Application(int argc, const char *argv[]);
    EXPORT ~Application();

    EXPORT void parseArguments();

    const ArgumentList& getRemainingArgs() { return remainingArguments; }

    /* Get the Application instance. */
    static Application* getApplication() { return app; }

    EXPORT std::shared_ptr<const BoolArgument> registerBoolArgument(const std::initializer_list<string>& names, const string& desc);
    EXPORT std::shared_ptr<const IntegerArgument> registerIntegerArgument(const std::initializer_list<string>& names, const string& desc);
    EXPORT std::shared_ptr<const DecimalArgument> registerDecimalArgument(const std::initializer_list<string>& names, const string& desc);
    EXPORT std::shared_ptr<const StringArgument> registerStringArgument(const std::initializer_list<string>& names, const string& desc);

    /* Get how long the application has been running. */
    template<typename T> T getRunningTime() {
        return  std::chrono::duration_cast<T>(Clock::now().time_since_epoch()) - std::chrono::duration_cast<T>(startTime);
    }

    const std::vector<Log::Output>& getLogOutputs() { return logOutputs; }
};

}
