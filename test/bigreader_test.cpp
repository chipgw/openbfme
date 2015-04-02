#include "application.hpp"
#include "bigreader.hpp"
#include "log.hpp"

using namespace OpenBFME;

bool testFile(const BigEntry* file){
    if(file == nullptr){
        Log::error("Unable to open file!");
        return false;
    }
    for(int i = 0; !file->eof(); ++i){
        Log::debug("Line #%04d: \"%s\"", i + 1, file->getLine(false));
    }
    return true;
}

int main(int argc, const char* argv[]){
    Application app(argc, argv);

    app.parseArguments();


    int failed = 0;

    const static std::vector<std::pair<string, string>> tests {
        {"folder/inifile.ini",  ""},
        {"inifile.ini",         "folder\\inifile.ini"},
        {"inifile.ini",         "folder/"},
        {"..\\NewFile.inc",     "folder\\inifile.ini"},
        {"..\\NewFile.inc",     "folder/inifile.ini"},
    };

    BigFilesystem big;

    big.mount("test.big", true);

    for(integer i = 0; i < tests.size(); ++i){
        Log::info("Test #%d:", i);
        if(!testFile(big.openFile(tests[i].first, tests[i].second)))
            failed++;
    }

    big.unmount("test.big");

    big.mount("test", true);

    for(integer i = 0; i < tests.size(); ++i){
        Log::info("Test #%d:", i + tests.size());
        if(!testFile(big.openFile(tests[i].first, tests[i].second)))
            failed++;
    }

    big.unmount("test");

    if(failed == 0)
        Log::info("All %i tests successful!", integer(tests.size()) * 2);
    else
        Log::error("%i/%i test(s) failed!", failed, integer(tests.size()) * 2);

    return 0;
}
