#include "argumentsystem.hpp"

namespace OpenBFME {

StringArgument::StringArgument(const std::initializer_list<string> &n, const string &d) : description(d), valid(false), names(n) {
    /* STUB */
}

void StringArgument::parse(){
    valid = result.size() != 0;
}

bool StringArgument::containsName(const string &name) const{
    return names.count(name) > 0;
}

void BoolArgument::parse() {
    if(result.size() == 0 || result == "yes" || result == "1"){
        boolResult = true;
        valid = true;
    }else if(result == "no" || result == "0"){
        boolResult = false;
        valid = true;
    }
}

void IntegerArgument::parse() {
    try{
        intResult = std::stoi(result);
        valid = true;
    }catch(...){}
}

void DecimalArgument::parse() {
    try{
        decResult = std::stof(result);
        valid = true;
    }catch(...){}
}


}
