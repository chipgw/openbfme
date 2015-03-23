#pragma once

#include "types.hpp"
#include <vector>
#include <memory>

namespace OpenBFME {

class StringArgument;

/* This class parses commandline and initializes the Log. */
class Application{
private:
    typedef std::vector<string> ArgumentList;

    /* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
    static Application* app;

    /* All the arguments passed to the application, minus the executable path itself. */
    ArgumentList fullArguments;

    /* Arguments handled by command line parser. */
    std::vector<std::shared_ptr<StringArgument>> parsedArguments;

    /* Any arguments not handled by the commandline parser. */
    ArgumentList remainingArguments;

    /* The path of the executable file that is running. */
    string executablePath;

public:
    EXPORT Application(int argc, const char *argv[]);
    EXPORT ~Application();

    EXPORT void parseArguments();

    const ArgumentList& getRemainingArgs() { return remainingArguments; }

    /* Get the Application instance. */
    static Application* getApplication() { return app; }

    template<typename T> std::shared_ptr<const T> registerArgument(const std::initializer_list<string>& names, const string& desc) {
        std::shared_ptr<T> pointer(new T(names, desc));
        parsedArguments.emplace_back(pointer);

        return pointer;
    }
};

}
