#include "argumentsystem.hpp"
#include "string.hpp"

namespace OpenBFME {

StringArgument::StringArgument(const std::initializer_list<string> &n, const string &d, bool expectValue) :
    description(d), valid(false), names(n), expectsValue(expectValue) {
    /* STUB */
}

void StringArgument::parse(const string &usedName){
    valid = result.size() != 0;
}

void StringArgument::printHelp(){
    string nameStr;

    for(const string& name : names){
        nameStr += ((name.size() == 1) ? "-" : "--") + name + ", ";
    }

    nameStr.erase(nameStr.size() - 2);
    nameStr += ':';

    /* Help messages are console only, with no timestamp. */
    puts(format("  %-20s\t%s", nameStr, description).c_str());
}

bool StringArgument::containsName(const string &name) const{
    return names.count(name) > 0;
}

void BoolArgument::parse(const string &usedName) {
    if(result.size() == 0 || result == "yes" || result == "1"){
        boolResult = true;
        valid = true;
    }else if(result == "no" || result == "0"){
        boolResult = false;
        valid = true;
    }else{
        errorMessage = format("Invalid command-line! expected \"yes\", \"no\", \"1\", or \"0\", after %s, got \"%s\"", usedName, result);
    }
}

void IntegerArgument::parse(const string& usedName) {
    try{
        intResult = std::stoi(result);
        valid = true;
    }catch(...){
        errorMessage = format("Invalid command-line! expected an integer value after %s, got \"%s\"", usedName, result);
    }
}

void DecimalArgument::parse(const string& usedName) {
    try{
        decResult = std::stof(result);
        valid = true;
    }catch(...){
        errorMessage = format("Invalid command-line! expected a decimal value after %s, got \"%s\"", usedName, result);
    }
}

void MultiStringArgument::parse(const string& usedName) {
    if (!result.empty()) {
        results.emplace(result);
    }
    valid = !results.empty();
}


}
