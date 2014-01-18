#ifndef OPENBFME_LOG_H
#define OPENBFME_LOG_H

#include <cstdint>
#include <cstdio>
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

    static void print(const char* format, LogOutputLevel level, va_list args);

public:
    static void init(const char* filename);

    static void info(const char* format,...);
    static void warning(const char* format,...);
    static void error(const char* format,...);
};

}

#endif // OPENBFME_LOG_H
