#pragma once

#include "types.hpp"
#include "string.hpp"
#include <list>

namespace OpenBFME {

class Log{
    enum LogOutputLevel{
        Error   = 0x01,
        Warning = 0x02,
        Info    = 0x04,
        Debug   = 0x08,
        All     = 0xff
    };

    typedef std::pair<LogOutputLevel, FILE*> LogOutput;
    static std::list<LogOutput> outputs;

    EXPORT static void print(const string& output, LogOutputLevel level);

public:
    EXPORT static void init(const char* filename, bool forceVerbose = false);

    inline static void debug(const string &output){
        print(output, LogOutputLevel::Debug);
    }
    template<typename First, typename ...Args> inline static void debug(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), LogOutputLevel::Debug);
    }

    inline static void info(const string &output){
        print(output, LogOutputLevel::Info);
    }
    template<typename First, typename ...Args> inline static void info(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), LogOutputLevel::Info);
    }

    inline static void warning(const string &output){
        print(output, LogOutputLevel::Warning);
    }
    template<typename First, typename ...Args> inline static void warning(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), LogOutputLevel::Warning);
    }

    inline static void error(const string &output){
        print(output, LogOutputLevel::Error);
    }
    template<typename First, typename ...Args> inline static void error(const string &fmt, First first, Args... args){
        print(format(fmt, {first, args...}), LogOutputLevel::Error);
    }
};

}
