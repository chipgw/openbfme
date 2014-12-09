#include "string.hpp"
#include <algorithm>
#include <cctype>

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

/*!
 * \brief Formats the string with a subset of printf syntax.
 * \todo Support more printf options.
 * \todo Make it more safe.
 */
string format(const string& fmt, std::initializer_list<Printable> args){
    string result;
    auto arg = args.begin();

    for(string::size_type i = 0; i < fmt.length(); ++i){
        if(fmt[i] == '%'){
            integer width = 0;
            integer precision = 0;

            bool usePrefix = false;
            bool zeroForPadding = false;

            if(fmt[++i] == '%'){
                result += fmt[i];
                continue;
            }
            while(!std::isalnum(fmt[i]) || fmt[i] == '0'){
                if(fmt[i] == '#')
                    usePrefix = true;
                else if(fmt[i] == '0')
                    zeroForPadding = true;
                ++i;
            }

            while(std::isdigit(fmt[i])){
                width *= 10;
                width += fmt[i] - '0';
                ++i;
            }

            if(fmt[i] == '.'){
                while(std::isdigit(fmt[++i])){
                    precision *= 10;
                    precision += fmt[i] - '0';
                }
            }

            const char* prefix = nullptr;
            string out;

            // TODO - handle errors and support more options.
            switch(fmt[i]){
            case 's':
                if(arg->type == Printable::String)
                    out = arg->str;
                break;
            case 'd':
            case 'i':
            case 'u':
                if(arg->type == Printable::Integer)
                    out = std::to_string(arg->num);
                break;
            case 'o':
                if(arg->type == Printable::Integer){
                    prefix = "0";
                    out = to_base(arg->num, 8);
                }
                break;
            case 'x':
                if(arg->type == Printable::Integer){
                    prefix = "0x";
                    out = to_base(arg->num, 16);
                }
                break;
            case 'X':
                if(arg->type == Printable::Integer){
                    prefix = "0X";
                    out = to_base(arg->num, 16, 'A');
                }
                break;
            case 'c':
                if(arg->type == Printable::Character)
                    out = arg->ch;
                break;
            case 'f':
            case 'F': // IDK what the difference is supposed to be...
                if(arg->type == Printable::Decimal)
                    out = std::to_string(arg->dec);
                break;
            }

            if(width > out.size()){
                out.insert(0, width - out.size(), zeroForPadding ? '0' : ' ');
            }

            if(prefix != nullptr && usePrefix){
                out.insert(0, prefix);
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
