#include "application.hpp"
#include "argumentsystem.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    auto boolArg = app.registerBoolArgument({"boolarg","b"}, "Test argument");
    auto intArg = app.registerIntegerArgument({"intarg","int","i"}, "Test argument");
    auto decArg = app.registerDecimalArgument({"decarg","d"}, "Test argument");
    auto strArg = app.registerStringArgument({"strarg","str"}, "Test argument");

    app.parseArguments();

    if(boolArg->valid)
        Log::info("--boolarg value is \"%s\"", boolArg->boolResult ? "true" : "false");
    else
        Log::info("No valid --boolarg was passed.");

    if(intArg->valid)
        Log::info("--intarg value is \"%i\"", intArg->intResult);
    else
        Log::info("No valid --intarg was passed.");

    if(decArg->valid)
        Log::info("--decarg value is \"%f\"", decArg->decResult);
    else
        Log::info("No valid --decarg was passed.");

    if(strArg->valid)
        Log::info("--strarg value is \"%s\"", strArg->result);
    else
        Log::info("No valid --strarg was passed.");

    auto& args = app.getRemainingArgs();

    if(args.size() > 0){
        Log::info("Arguments:");

        for(const string& arg : args){
            Log::info(arg);
        }
    }

    /* Check the error message. */
    Log::info("Now creating a second instance of Application, you should see an error message as that's against the rules.");
    Application app2(argc, argv);

    return 0;
}
