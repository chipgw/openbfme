#pragma once

#include "types.hpp"
#include <vector>
#include <unordered_set>
#include <memory>

namespace OpenBFME {

class ArgumentDef{
public:
    enum ArgumentType{
        Bool,
        Integer,
        Decimal,
        String,
    };

private:
    std::unordered_set<string> names;

public:
    const string description;

    const ArgumentType type;
    bool valid;

    union{
        bool boolResult;
        integer intResult;
        decimal decResult;
    };
    string result;

    ArgumentDef(ArgumentType t, const string& n, const string& d);

    void parse();

    bool containsName(const string& name) const;

    /* No copying thank you very much. */
    ArgumentDef(const ArgumentDef&) = delete;
    ArgumentDef& operator=(const ArgumentDef&) = delete;
};

/* This class parses commandline and initializes the Log. */
class Application{
private:
    typedef std::vector<string> ArgumentList;

    /* Only one instance of this class is allowed, to be created in main and provided with the command line arguments. */
    static Application* app;

    /* All the arguments passed to the application, minus the executable path itself. */
    ArgumentList fullArguments;

    /* Arguments handled by command line parser. */
    std::vector<std::shared_ptr<ArgumentDef>> parsedArguments;

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

    EXPORT std::shared_ptr<const ArgumentDef> registerArgument(ArgumentDef::ArgumentType type, const string& names, const string& desc);
};

}
