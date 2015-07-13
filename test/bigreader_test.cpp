#include "log.hpp"
#include "application.hpp"
#include "bigentry.hpp"
#include "bigfilesystem.hpp"

using namespace OpenBFME;

bool testFile(const BigEntry* file) {
    if (file == nullptr) {
        Log::error("Unable to open file!");
        return false;
    }

    while (!file->eof())
        Log::debug("Line #%04d: \"%s\"", file->getLineNumber(), file->getLine(false));

    Log::info("Test successful.");
    return true;
}

void testWildcard() {
    auto files = BigFilesystem::findFiles("data/ini/*.ini");

    for (const string& file : files)
        Log::info("Found file matching wildcard: \"%s\"", file);
}

int main(int argc, const char* argv[]) {
    Application app(argc, argv);

    app.parseArguments();

    integer failed = 0;
    integer totaltests = 0;

    const static std::vector<std::pair<string, string>> tests {
        {"folder/inifile.ini",  ""},
        {"inifile.ini",         "folder\\inifile.ini"},
        {"inifile.ini",         "folder/"},
        {"..\\NewFile.inc",     "folder\\inifile.ini"},
        {"..\\NewFile.inc",     "folder/inifile.ini"},
    };

    BigFilesystem::mount("test.big", true);

    for (integer i = 0; i < tests.size(); ++i) {
        Log::info("Test #%d:", totaltests++);
        if(!testFile(BigFilesystem::openFile(tests[i].first, tests[i].second)))
            ++failed;
    }

    testWildcard();

    BigFilesystem::unmount("test.big");

    auto archive = BigFilesystem::mount("test", true);

    for (integer i = 0; i < tests.size(); ++i) {
        Log::info("Test #%d:", totaltests++);
        if (!testFile(BigFilesystem::openFile(tests[i].first, tests[i].second)))
            ++failed;
    }

    testWildcard();

    Log::info("Test #%d:", totaltests++);
    if (archive->writeBig("writetest.big"))
        Log::info("Test successful.");
    else
        ++failed;

    BigFilesystem::unmount(archive);

    BigFilesystem::mount("writetest.big", true);

    for (integer i = 0; i < tests.size(); ++i) {
        Log::info("Test #%d:", totaltests++);
        if (!testFile(BigFilesystem::openFile(tests[i].first, tests[i].second)))
            ++failed;
    }

    testWildcard();

    BigFilesystem::unmount("writetest.big");

    if (failed == 0)
        Log::info("All %i tests successful!", totaltests);
    else
        Log::error("%i/%i test(s) failed!", failed, totaltests);

    return 0;
}
