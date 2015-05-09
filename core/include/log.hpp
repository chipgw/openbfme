#pragma once

#include "types.hpp"
#include "string.hpp"

namespace OpenBFME {

namespace Log {

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

EXPORT void print(const string& output, OutputLevel level);

inline void debug(const string &output){
    print(output, OutputLevel::Debug);
}
template<typename First, typename ...Args> inline void debug(const string &fmt, First first, Args... args){
    print(format(fmt, {first, args...}), OutputLevel::Debug);
}

inline void info(const string &output){
    print(output, OutputLevel::Info);
}
template<typename First, typename ...Args> inline void info(const string &fmt, First first, Args... args){
    print(format(fmt, {first, args...}), OutputLevel::Info);
}

inline void warning(const string &output){
    print(output, OutputLevel::Warning);
}
template<typename First, typename ...Args> inline void warning(const string &fmt, First first, Args... args){
    print(format(fmt, {first, args...}), OutputLevel::Warning);
}

inline void error(const string &output){
    print(output, OutputLevel::Error);
}
template<typename First, typename ...Args> inline void error(const string &fmt, First first, Args... args){
    print(format(fmt, {first, args...}), OutputLevel::Error);
}

}

}
