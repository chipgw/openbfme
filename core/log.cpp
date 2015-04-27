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
        WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
#define RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#else
        const char* color = "\033[0m";
#define RED "\033[22;31m"
#define YELLOW "\033[01;33m"
#endif

        switch (level) {
        case Error:
            type = "ERROR";
            color = RED;
            break;
        case Warning:
            type = "WARNING";
            color = YELLOW;
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
            if(output.fp != nullptr && output.level & level){
#ifdef OPENBFME_PLATFORM_WINDOWS
                if(output.fp == stderr)
                    SetConsoleTextAttribute(GetStdHandle(STD_ERROR_HANDLE), color);
                else if(output.fp == stdout)
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
                if(output.fp == stderr || output.fp == stdout)
                    fputs(color, output.fp);
#endif

                fputs(timestamp.c_str(), output.fp);
                fputs(str.c_str(), output.fp);
                fputc('\n', output.fp);
                fflush(output.fp);
            }
        }
    }
}

}
