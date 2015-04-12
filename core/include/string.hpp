#pragma once

#include "types.hpp"
#include <initializer_list>

namespace OpenBFME {

struct Printable{
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
    inline Printable(const string& s) : type(String), str(s.c_str()) {}
    inline Printable(cstring s) : type(String), str(s) {}
};

EXPORT string to_base(unsigned int value, unsigned int base, char start = 'a');

EXPORT string format(const string& fmt, std::initializer_list<Printable> args);

/*!
 * \brief Formats the string with a subset of printf syntax.
 * \todo Support more printf options.
 * \todo Make it more safe.
 */
template<typename First, typename... Args> inline string format(const string& fmt, First first, Args... args){
    return format(fmt, { first, args... });
}

}
