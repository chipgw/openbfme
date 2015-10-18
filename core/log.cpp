#include "log.hpp"
#include "application.hpp"

#ifdef OPENBFME_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace OpenBFME {

namespace Log {

namespace {
/* A nice private list of FILE*'s and their associated level. */
struct Output {
    OutputLevel level;
    FILE* fp;

    Output(OutputLevel l, FILE* f) : level(l), fp(f) { }
};

std::vector<Output> logOutputs;
}

void initLog(const string& filename, bool verbose, bool silent) {
    OutputLevel maxLevel = silent ? Error : verbose ? Debug : Info;

    logOutputs.push_back(Output(maxLevel, stdout));

    FILE* file = fopen(filename.c_str(), "w");
    if(file != nullptr)
        logOutputs.push_back(Output(maxLevel, file));
}

void shutdownLog() {
    /* Close all files that aren't stdout. */
    for(Output& output : logOutputs)
        if(output.fp != stdout) fclose(output.fp);

    logOutputs.clear();
}

void print(const string& str, OutputLevel level) {
    Application* app = Application::getApplication();

    if(app != nullptr) {
        const char* type;

#ifdef OPENBFME_PLATFORM_WINDOWS
        /* This is how we set the console color on Windows. */
#define SET_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);

        /* These are the valid console colors on Windows. */
        const WORD BLANK  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        const WORD RED    = FOREGROUND_RED | FOREGROUND_INTENSITY;
        const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
#else
        /* This is how we set the console color on Linux. */
#define SET_COLOR(color) fputs(color, stdout)

        /* These are the valid console colors on Linux. */
        const char* BLANK   = "\033[0m";
        const char* RED     = "\033[22;31m";
        const char* YELLOW  = "\033[01;33m";
#endif

        switch (level) {
        case Error:
            type = "ERROR";
            SET_COLOR(RED);
            break;
        case Warning:
            type = "WARNING";
            SET_COLOR(YELLOW);
            break;
        case Debug:
            type = "DEBUG";
            break;
        case Info:
        default:
            type = "INFO";
            break;
        }

        using namespace std::chrono;

        /* Make the timestamp string. */
        auto current = app->getRunningTime<microseconds>();

        string timestamp = format("[%02i:%02i.%06i] %s: ",
                                  duration_cast<minutes>(current).count(),
                                  duration_cast<seconds>(current).count() % 60,
                                  current.count() % 1000000,
                                  type);

        /* Write the string to every relevant output. */
        for (Output& output : logOutputs) {
            if (output.fp != nullptr && output.level >= level) {
                fputs(timestamp.c_str(), output.fp);
                fputs(str.c_str(), output.fp);
                fputc('\n', output.fp);
                fflush(output.fp);
            }
        }

        SET_COLOR(BLANK);
    }
}

}

}
