#include "application.hpp"
#include <sstream>

namespace OpenBFME {

ArgumentDef::ArgumentDef(ArgumentType t, const string &n, const string &d) : type(t), description(d), valid(false) {
    std::stringstream namesStream(n);

    while(namesStream.good()) {
        string name;
        std::getline(namesStream, name, ',');
        names.emplace(name);
    }
}

void ArgumentDef::parse(){
    switch(type){
    case Bool:
        if(result.size() == 0 || result == "yes" || result == "1"){
            boolResult = true;
            valid = true;
        }

        if(result == "no" || result == "0"){
            boolResult = false;
            valid = true;
        }
        break;
    case Integer:{
        try{
            intResult = std::stoi(result);
            valid = true;
        }catch(...){}
        break;
    }
    case Decimal:{
        try{
            decResult = std::stof(result);
            valid = true;
        }catch(...){}
        break;
    }
    case String:
        valid = true;
        break;
    }
}

bool ArgumentDef::containsName(const string &name) const{
    return names.count(name) > 0;
}

}
