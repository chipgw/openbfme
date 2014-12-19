#include "log.hpp"
#include <chrono>

using namespace std;

namespace OpenBFME {

void Log::init(const char *filename, bool forceVerbose){
    if(outputs.size() < 1){
#ifndef NDEBUG
        /* Debug output is always on in debug builds. */
        forceVerbose = true;
#endif
        outputs.push_back(LogOutput(LogOutputLevel(Info | Warning | (forceVerbose ? Debug : 0)), stdout));
        outputs.push_back(LogOutput(Error, stderr));

        FILE* file = fopen(filename, "w");
        if(file != nullptr){
            outputs.push_back(LogOutput(LogOutputLevel(All & forceVerbose ? ~0 : ~Debug), file));
        }
    }
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

    chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();

    time_t tnow = chrono::high_resolution_clock::to_time_t(current);
    tm *date = localtime(&tnow);
    int microseconds = chrono::duration_cast<chrono::microseconds>(current.time_since_epoch()).count() % 1000000;
    string timestamp = format("[%02i:%02i:%02i.%06i] %s: ", date->tm_hour, date->tm_min, date->tm_sec, microseconds, type);

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
