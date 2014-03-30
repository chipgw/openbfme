#ifndef OPENBFME_STRING_HPP
#define OPENBFME_STRING_HPP

#include "types.hpp"
#include <vector>

namespace OpenBFME {

struct Printable{
    // TODO - support more stuffs.
    enum Type{
        Integer, Decimal, Character, String
    };
    Type type;
    union {
        integer num;
        decimal dec;
        character ch;
        cstring str;
    };

    template<typename T> Printable(T n, typename std::enable_if<std::is_integral<T>::value>::type* = 0) : type(Integer), num(n) {}
    template<typename T> Printable(T d, typename std::enable_if<std::is_floating_point<T>::value>::type* = 0) : type(Decimal), dec(d) {}
    inline Printable(char c) : type(Character), ch(c) {}
    inline Printable(const std::string& s) : type(String), str(s.c_str()) {}
    inline Printable(cstring s) : type(String), str(s) {}
};

EXPORT string to_base(unsigned int value, unsigned int base, char start = 'a');

inline string format(const string& fmt){ return fmt; }

EXPORT string format(const string& fmt, std::vector<Printable> args);

/*!
 * \brief Formats the string with a subset of printf syntax.
 * \todo Support more printf options.
 * \todo Make it more safe.
 */
template<typename... Args> inline string format(const string& fmt, Args... args){
    return format(fmt, std::vector<Printable>{ args... });
}

}

#endif //OPENBFME_STRING_HPP
