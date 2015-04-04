#include "log.hpp"
#include "application.hpp"

namespace OpenBFME {

void Log::init(const string& filename, bool verbose, bool silent){
    if(outputs.size() < 1){
#ifndef NDEBUG
        /* Debug output is always on in debug builds. */
        verbose = true;
#endif
        if(!silent)
            outputs.push_back(LogOutput(LogOutputLevel(Info | Warning | (verbose ? Debug : 0)), stdout));

        outputs.push_back(LogOutput(Error, stderr));

        FILE* file = fopen(filename.c_str(), "w");
        if(file != nullptr){
            outputs.push_back(LogOutput(silent ? Error : LogOutputLevel(All & (verbose ? ~0 : ~Debug)), file));
        }
    }
}

void Log::shutdown(){
    for(LogOutput output : outputs)
        if(output.second != stdout && output.second != stderr)
            fclose(output.second);

    outputs.clear();
}

void Log::print(const string& str, LogOutputLevel level){
    const char* type;

    switch (level) {
    case Error:
        type = "ERROR";
        break;
    case Warning:
        type = "WARNING";
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
    auto current = Application::getApplication()->getRunningTime<microseconds>();

    string timestamp = format("[%02i:%02i.%06i] %s: ",
                              duration_cast<minutes>(current).count(),
                              duration_cast<seconds>(current).count() % 60,
                              current.count() % 1000000,
                              type);

    /* Write the string to every relevant output. */
    for(LogOutput output : outputs){
        if(output.first & level){
            fputs(timestamp.c_str(), output.second);
            fputs(str.c_str(), output.second);
            fputc('\n', output.second);
            fflush(output.second);
        }
    }
}

std::list<Log::LogOutput> Log::outputs = std::list<Log::LogOutput>();

}
