#pragma once

#include "types.hpp"
#include <vector>
#include <unordered_map>

namespace OpenBFME {

/* This class parses commandline and initializes the Log. */
class Application{
private:
    typedef std::vector<string> ArgumentList;

    /* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
    static Application* app;

    /* All the arguments passed to the application, minus the executable path itself. */
    ArgumentList fullArguments;

    /* Arguments handled by command line parser. i.e. any argument in the form: "--<key>[=<value>]". */
    std::unordered_map<string, string> parsedArguments;

    /* Any arguments not handled by the commandline parser. */
    ArgumentList remainingArguments;

    /* The path of the executable file that is running. */
    string executablePath;

public:
    EXPORT Application(int argc, const char *argv[]);

    const ArgumentList& getRemainingArgs() { return remainingArguments; }

    /* Get the Application instance. */
    static Application* getApplication() { return app; }

    /* Get the value of an argument. */
    EXPORT bool getBoolArgument(string name, bool* valid = nullptr);
    EXPORT integer getIntegerArgument(string name, bool* valid = nullptr);
    EXPORT decimal getDecimalArgument(string name, bool* valid = nullptr);
    EXPORT string getStringArgument(string name, bool* valid = nullptr);
};

}
