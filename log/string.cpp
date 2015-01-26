#include "string.hpp"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace OpenBFME {

string to_base(unsigned int value, unsigned int base, char start) {
    if (value == 0) return "0";

    string result;

    while (value != 0) {
        uint8_t d = value % base;
        result += (d < 10 ? d + '0' : start + d - 10);
        value /= base;
    }

    std::reverse(result.begin(), result.end());
    return result;
}

string format(const string& fmt, std::initializer_list<Printable> args){
    string result;
    auto arg = args.begin();

    for(string::size_type i = 0; i < fmt.length(); ++i){
        if(fmt[i] == '%'){
            integer width = 0;
            integer precision = -1;

            bool usePrefix = false;
            bool zeroForPadding = false;
            bool showSign = false;
            bool leftJustify = false;

            if(fmt[++i] == '%'){
                result += fmt[i];
                continue;
            }
            while((!std::isalnum(fmt[i]) || fmt[i] == '0') && fmt[i] != '.'){
                if(fmt[i] == '#')
                    usePrefix = true;
                else if(fmt[i] == '0')
                    zeroForPadding = true;
                else if(fmt[i] == '+')
                    showSign = true;
                else if(fmt[i] == '-')
                    leftJustify = true;
                ++i;
            }

            while(std::isdigit(fmt[i])){
                width *= 10;
                width += fmt[i] - '0';
                ++i;
            }

            if(fmt[i] == '.'){
                precision = 0;
                while(std::isdigit(fmt[++i])){
                    precision *= 10;
                    precision += fmt[i] - '0';
                }
            }

            string prefix;
            string out;

            switch(fmt[i]){
            case 's':
                if(arg->type == Printable::String){
                    out = arg->str;
                    if(precision > -1 && out.size() > precision)
                        out.erase(precision, string::npos);
                }
                break;
            case 'd':
            case 'i':
            case 'u':
                if(arg->type == Printable::Integer){
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
            case 'f':
            case 'F': /* IDK what the difference is supposed to be... */
                if(arg->type == Printable::Decimal){
                    std::ostringstream floatStr;
                    floatStr << std::fixed << std::setprecision(precision) << arg->dec;
                    out = floatStr.str();
                }
                break;
            case 'e':
            case 'E':
                if(arg->type == Printable::Decimal){
                    std::ostringstream floatStr;
                    if(fmt[i] == 'E')
                        floatStr << std::uppercase;
                    floatStr << std::fixed << std::scientific << std::setprecision(precision) << arg->dec;
                    out = floatStr.str();
                }
                break;
            }

            out.insert(0, prefix);

            if(width > out.size()){
                out.insert(zeroForPadding ? prefix.size() : leftJustify ? out.size() : 0, width - out.size(), zeroForPadding ? '0' : ' ');
            }

            result += out;
            ++arg;
        }else{
            result += fmt[i];
        }
    }

    return result;
}

}
