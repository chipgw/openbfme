#include "log.hpp"
#include "application.hpp"

#ifdef OPENBFME_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace OpenBFME {

void Log::print(const string& str, OutputLevel level){
    Application* app = Application::getApplication();

    if(app != nullptr){
        const char* type;

#ifdef OPENBFME_PLATFORM_WINDOWS
#define SET_COLOR(color) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
        const WORD BLANK  = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        const WORD RED    = FOREGROUND_RED | FOREGROUND_INTENSITY;
        const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
#else
#define SET_COLOR(color) fputs(color, stdout)
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
        for(Output output : app->getLogOutputs()){
            if(output.fp != nullptr && output.level >= level){
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
