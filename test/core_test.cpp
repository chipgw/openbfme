#include "application.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    bool boolValid, intValid, decValid, strValid;

    bool boolArg = app.getBoolArgument("boolarg", &boolValid);
    integer intArg = app.getIntegerArgument("intarg", &intValid);
    decimal decArg = app.getDecimalArgument("decarg", &decValid);
    string strArg = app.getStringArgument("strarg", &strValid);

    if(boolValid)
        Log::info("--boolarg value is \"%s\"", boolArg ? "true" : "false");
    else
        Log::info("No valid --boolarg was passed.");

    if(intValid)
        Log::info("--intarg value is \"%i\"", intArg);
    else
        Log::info("No valid --intarg was passed.");

    if(decValid)
        Log::info("--decarg value is \"%f\"", decArg);
    else
        Log::info("No valid --decarg was passed.");

    if(strValid)
        Log::info("--strarg value is \"%s\"", strArg);
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
