#ifndef OPENBFME_LOG_HPP
#define OPENBFME_LOG_HPP

#include "types.hpp"
#include "string.hpp"
#include <list>

namespace OpenBFME {

class LogOutput;

enum class LogOutputLevel : uint8_t {
    Error   = 0x01,
    Warning = 0x02,
    Info    = 0x04,
    All     = 0xff
};
constexpr inline LogOutputLevel operator |(LogOutputLevel a, LogOutputLevel b){
    typedef std::underlying_type<LogOutputLevel>::type enum_type;
    return static_cast<LogOutputLevel>(static_cast<enum_type>(a) | static_cast<enum_type>(b));
}
constexpr inline bool operator &(LogOutputLevel a, LogOutputLevel b){
    typedef std::underlying_type<LogOutputLevel>::type enum_type;
    return static_cast<enum_type>(a) & static_cast<enum_type>(b);
}

class Log{
    static std::list<LogOutput> outputs;

    static void print(const string& output, LogOutputLevel level);

public:
    static void init(const char* filename);

    template<typename ...Args> inline static void info(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Info);
    }

    template<typename ...Args> inline static void warning(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Info);
    }

    template<typename ...Args> inline static void error(const string &fmt, Args... args){
        print(format(fmt, args...), LogOutputLevel::Info);
    }
};

}

#endif // OPENBFME_LOG_HPP
