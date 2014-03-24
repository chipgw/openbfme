#ifndef OPENBFME_LOG_HPP
#define OPENBFME_LOG_HPP

#include "types.hpp"
#include "string.hpp"
#include <list>

namespace OpenBFME {

class Log{
    class LogOutput;

    enum LogOutputLevel{
        Error   = 0x01,
        Warning = 0x02,
        Info    = 0x04,
        All     = 0xff
    };

    static std::list<LogOutput> outputs;

    static void print(const string& output, LogOutputLevel level);

public:
    static void init(const char* filename);

    template<typename ...Args> inline static void info(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Info);
    }

    template<typename ...Args> inline static void warning(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Warning);
    }

    template<typename ...Args> inline static void error(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Error);
    }
};

}

#endif // OPENBFME_LOG_HPP
