#pragma once

#include "types.hpp"
#include "string.hpp"

namespace OpenBFME {

class Log{
public:
    enum OutputLevel{
        Error,
        Warning,
        Info,
        Debug
    };
    struct Output{
        OutputLevel level;
        FILE* fp;

        Output(OutputLevel l, FILE* f) : level(l), fp(f) {}
    };

    EXPORT static void print(const string& output, OutputLevel level);

    inline static void debug(const string &output){
        print(output, OutputLevel::Debug);
    }
    template<typename First, typename ...Args> inline static void debug(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), OutputLevel::Debug);
    }

    inline static void info(const string &output){
        print(output, OutputLevel::Info);
    }
    template<typename First, typename ...Args> inline static void info(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), OutputLevel::Info);
    }

    inline static void warning(const string &output){
        print(output, OutputLevel::Warning);
    }
    template<typename First, typename ...Args> inline static void warning(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), OutputLevel::Warning);
    }

    inline static void error(const string &output){
        print(output, OutputLevel::Error);
    }
    template<typename First, typename ...Args> inline static void error(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), OutputLevel::Error);
    }
};

}
