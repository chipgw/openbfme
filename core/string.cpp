#include "string.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace OpenBFME {

/* Convert an unsigned integer to the specified base, with start being the character used after 9. */
string to_base(uint32_t value, uint8_t base, character start = 'a') {
    if (value == 0) return "0";

    /* So that digit + start is the original value when the digit is 10. */
    start -= 10;

    string result;

    while (value != 0) {
        uint8_t digit = value % base;
        result += digit + (digit < 10 ? '0' : start);
        value /= base;
    }

    /* The string is constructed backwards, so we fix it. */
    std::reverse(result.begin(), result.end());
    return result;
}

bool stringCaseInsensitiveEquals(const string& a, const string& b) {
    return a.size() == b.size() ? std::equal(a.cbegin(), a.cend(), b.cbegin(), [](character a, character b){
        return tolower(a) == tolower(b); 
    }) : false;
}

string format(const string& fmt, std::initializer_list<Printable> args){
    string result;
    auto arg = args.begin();

    for(string::size_type i = 0; i < fmt.length(); ++i){
        if(fmt[i] == '%' && fmt[++i] != '%'){
            integer width = 0;
            integer precision = -1;

            bool usePrefix = false;
            bool zeroForPadding = false;
            bool showSign = false;
            bool leftJustify = false;

            /* Check for flags, stop on a letter, number (excluding 0), or '.' */
            while((!std::isalnum(fmt[i]) || fmt[i] == '0') && fmt[i] != '.'){
                switch (fmt[i]) {
                case '#':
                    usePrefix = true;
                    break;
                case '0':
                    zeroForPadding = true;
                    break;
                case '+':
                    showSign = true;
                    break;
                case '-':
                    leftJustify = true;
                    break;
                default:
                    break;
                }
                ++i;
            }

            /* Digits following the flags are used for the width. */
            while(std::isdigit(fmt[i])){
                width *= 10;
                width += fmt[i] - '0';
                ++i;
            }

            /* Digits after a '.' are used for precision. */
            if(fmt[i] == '.'){
                precision = 0;
                while(std::isdigit(fmt[++i])){
                    precision *= 10;
                    precision += fmt[i] - '0';
                }
            }

            /* We use a ostringstream for float values to allow specifying precision. */
            std::ostringstream floatStr;
            string prefix;
            string out;

            switch(fmt[i]){
            case 's':
                if(arg->type == Printable::String){
                    out = arg->str;
                    /* With a string precision sets the maximum output length. */
                    if(precision > -1 && out.size() > precision)
                        out.erase(precision, string::npos);
                }
                break;
            case 'd':
            case 'i':
            case 'u':
                if(arg->type == Printable::Integer){
                    /* We use the absolute value and handle the sign using the prefix string,
                     * because otherwise it ends up between the number and padding zeroes. */
                    out = std::to_string(abs(arg->num));
                    if(arg->num < 0)
                        prefix = "-";
                    else if(showSign)
                        prefix = "+";
                }
                break;
            case 'o':
                if(arg->type == Printable::Integer){
                    if(usePrefix)
                        prefix = "0";
                    out = to_base(arg->num, 8);
                }
                break;
            case 'x':
                if(arg->type == Printable::Integer){
                    if(usePrefix)
                        prefix = "0x";
                    out = to_base(arg->num, 16);
                }
                break;
            case 'X':
                if(arg->type == Printable::Integer){
                    if(usePrefix)
                        prefix = "0X";
                    out = to_base(arg->num, 16, 'A');
                }
                break;
            case 'c':
                if(arg->type == Printable::Character)
                    out = arg->ch;
                break;
            case 'F':
                /* I honestly have no clue if this actually makes any difference... */
                floatStr << std::uppercase;
            case 'f':
                if(arg->type == Printable::Decimal){
                    floatStr << std::fixed << std::setprecision(precision) << arg->dec;
                    out = floatStr.str();
                }
                break;
            case 'E':
                floatStr << std::uppercase;
            case 'e':
                if(arg->type == Printable::Decimal){
                    floatStr << std::fixed << std::scientific << std::setprecision(precision) << arg->dec;
                    out = floatStr.str();
                }
                break;
            case 'G':
                floatStr << std::uppercase;
            case 'g':
                if(arg->type == Printable::Decimal){
                    floatStr << std::setprecision(precision) << arg->dec;
                    out = floatStr.str();
                }
                break;
            }

            /* Add the prefix string to the start of the output.
             * This is used for +/- signs as well as "0x", so usePrefix is checked when setting the prefix string. */
            out.insert(0, prefix);

            /* If the string is too short, add to it. */
            if(width > out.size()){
                if(zeroForPadding)
                    out.insert(prefix.size(), width - out.size(), '0');
                else
                    out.insert(leftJustify ? out.size() : 0, width - out.size(), ' ');
            }

            /* This argument is done, continue to the next one... */
            result += out;
            ++arg;
        }else{
            /* Just a plain character... */
            result += fmt[i];
        }
    }

    return result;
}

}
