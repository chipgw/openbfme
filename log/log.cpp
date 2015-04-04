#include "log.hpp"
#include <chrono>

using namespace std;

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

    /* Make the timestamp string. */
    auto current = chrono::system_clock::now();
    time_t tnow = chrono::system_clock::to_time_t(current);
    tm *date = localtime(&tnow);
    int microseconds = chrono::duration_cast<chrono::microseconds>(current.time_since_epoch()).count() % 1000000;
    string timestamp = format("[%02i:%02i:%02i.%06i] %s: ", date->tm_hour, date->tm_min, date->tm_sec, microseconds, type);

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

list<Log::LogOutput> Log::outputs = list<Log::LogOutput>();

}
