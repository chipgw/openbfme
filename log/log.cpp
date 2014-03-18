#include "log.hpp"
#include <chrono>
#include <cstdarg>

using namespace std;

namespace OpenBFME {

class LogOutput{
public:
    LogOutputLevel level;
    FILE* output;

    LogOutput(LogOutputLevel l, FILE* o) : level(l), output(o) {}
};

void Log::init(const char *filename){
    if(outputs.size() < 1){
        outputs.push_back(LogOutput(LogOutputLevel::Info | LogOutputLevel::Warning, stdout));
        outputs.push_back(LogOutput(LogOutputLevel::Error, stderr));

        FILE* file = fopen(filename, "w");
        if(file != nullptr){
            outputs.push_back(LogOutput(LogOutputLevel::All, file));
        }
    }
}

void Log::info(const char *format,...){
    va_list args;
    va_start(args, format);

    print(format, LogOutputLevel::Info, args);

    va_end(args);
}

void Log::warning(const char *format,...){
    va_list args;
    va_start(args, format);

    print(format, LogOutputLevel::Warning, args);

    va_end(args);
}

void Log::error(const char *format,...){
    va_list args;
    va_start(args, format);

    print(format, LogOutputLevel::Error, args);

    va_end(args);
}

void Log::print(const char *format, LogOutputLevel level, va_list args){
    // TODO - do a better job of this.
    const char* type;

    switch (level) {
    case LogOutputLevel::Error:
        type = "ERROR";
        break;
    case LogOutputLevel::Warning:
        type = "WARNING";
        break;
    case LogOutputLevel::Info:
    default:
        type = "INFO";
        break;
    }

    std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();

    time_t tnow = chrono::high_resolution_clock::to_time_t(current);
    tm *date = localtime(&tnow);
    int microseconds = chrono::duration_cast<chrono::microseconds>(current.time_since_epoch()).count() % 1000000;

    for(LogOutput output : outputs){
        if(output.level & level){
            va_list copy;
            va_copy(copy, args);

            fprintf(output.output, "[%02i:%02i:%02i.%06i] %s: ", date->tm_hour, date->tm_min, date->tm_sec, microseconds, type);
            vfprintf(output.output, format, copy);
            fprintf(output.output, "\n");
            fflush(output.output);

            va_end(copy);
        }
    }
}

list<LogOutput> Log::outputs = list<LogOutput>();

}
