#include "application.hpp"
#include "bigreader.hpp"

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

    BigFilesystem::mount("test.big", true);

    for(integer i = 0; i < tests.size(); ++i){
        Log::info("Test #%d:", i);
        if(!testFile(BigFilesystem::openFile(tests[i].first, tests[i].second)))
            failed++;
    }

    BigFilesystem::unmount("test.big");

    auto archive = BigFilesystem::mount("test", true);

    for(integer i = 0; i < tests.size(); ++i){
        Log::info("Test #%d:", i + integer(tests.size()));
        if(!testFile(BigFilesystem::openFile(tests[i].first, tests[i].second)))
            failed++;
    }

    BigFilesystem::unmount(archive);

    if(failed == 0)
        Log::info("All %i tests successful!", integer(tests.size()) * 2);
    else
        Log::error("%i/%i test(s) failed!", failed, integer(tests.size()) * 2);

    return 0;
}
