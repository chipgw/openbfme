#pragma once

#include "types.hpp"
#include <vector>

namespace OpenBFME {

/* This class parses commandline and initializes the Log. */
class Application{
private:
    typedef std::vector<string> ArgumentList;

    /* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
    static Application* app;

    /* All the arguments passed to the application, minus the executable path itself. */
    ArgumentList fullArguments;

    /* Any arguments not handled by the commandline parser. */
    ArgumentList remainingArguments;

    /* The path of the executable file that is running. */
    string executablePath;

public:
    EXPORT Application(int argc, const char *argv[]);

    const ArgumentList& getRemainingArgs() { return remainingArguments; }

    /* Get the Application instance. */
    static Application* getApplication() { return app; }
};

}
