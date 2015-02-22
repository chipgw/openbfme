#include "application.hpp"
#include "log.hpp"

using namespace OpenBFME;

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

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
