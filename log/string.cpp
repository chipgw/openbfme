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
string format(const string& fmt, std::vector<Printable> args){
    string result;
    integer arg = 0;

    // TODO - maybe split the rest of this into a single non-template function?
    for(string::size_type i = 0; i < fmt.length(); ++i){
        if(fmt[i] == '%'){
            string flags;
            integer width = 0;
            integer precision = 0;

            if(fmt[++i] == '%'){
                result += fmt[i];
                continue;
            }
            while(!std::isalnum(fmt[i]) || fmt[i] == '0'){
                flags += fmt[i];
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

            const char &type = fmt[i];

            string out;

            // TODO - handle errors and support more options.
            switch(type){
            case 's':
                if(args[arg].type == Printable::String)
                    out = args[arg].str;
                break;
            case 'd':
            case 'i':
                if(args[arg].type == Printable::Integer)
                    out = std::to_string(args[arg].num);
                break;
            case 'o':
                if(args[arg].type == Printable::Integer)
                    out = to_base(args[arg].num, 8);
                break;
            case 'x':
                if(args[arg].type == Printable::Integer)
                    out = to_base(args[arg].num, 16);
                break;
            case 'X':
                if(args[arg].type == Printable::Integer)
                    out = to_base(args[arg].num, 16, 'A');
                break;
            case 'c':
                if(args[arg].type == Printable::Character)
                    out = args[arg].ch;
                break;
            case 'f':
            case 'F': // IDK what the difference is supposed to be...
                if(args[arg].type == Printable::Decimal)
                    out = std::to_string(args[arg].dec);
                break;
            }

            if(width > out.size()){
                out.insert(0, width - out.size(), (flags.find_first_of('0') != string::npos) ? '0' : ' ');
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
