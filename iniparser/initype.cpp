#include "log.hpp"
#include "inivariable.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include <algorithm>

namespace OpenBFME {

namespace {

IniType makeDefinitionType(integer recursion){
    IniType type;

    type.variableTypes.emplace("BoolVariable",       IniVariable::String);
    type.variableTypes.emplace("IntegerVariable",    IniVariable::String);
    type.variableTypes.emplace("DecimalVariable",    IniVariable::String);
    type.variableTypes.emplace("PercentVariable",    IniVariable::String);
    type.variableTypes.emplace("StringVariable",     IniVariable::String);
    type.variableTypes.emplace("ColorVariable",      IniVariable::String);
    type.variableTypes.emplace("VectorVariable",     IniVariable::String);
    type.variableTypes.emplace("LineVariable",       IniVariable::String);

    if(recursion > 0)
        type.subTypes.emplace("SubType", makeDefinitionType(recursion - 1));

    return type;
}

const IniType IniDefinitionType = makeDefinitionType(16);

}

IniType::IniType(const string& filename) {
    loadFromIni(filename);
}

IniType::IniType(const IniObject& object) {
    loadFromIni(object);
}

bool IniType::loadFromIni(const string& filename) {
    IniParser ini;
    if(const BigEntry* file = BigFilesystem::openFile(filename)){
        IniObject root(IniDefinitionType);
        ini.parse(*file, root);

        return loadFromIni(root);
    }
    Log::warning("Unable to open file!");
    return false;
}

bool IniType::loadFromIni(const IniObject &object) {
    for(auto& var : object.variables){
        if(var.first == "BoolVariable")
            variableTypes[var.second.s] = IniVariable::Bool;
        else if(var.first == "IntegerVariable")
            variableTypes[var.second.s] = IniVariable::Integer;
        else if(var.first == "DecimalVariable")
            variableTypes[var.second.s] = IniVariable::Decimal;
        else if(var.first == "PercentVariable")
            variableTypes[var.second.s] = IniVariable::Percent;
        else if(var.first == "StringVariable")
            variableTypes[var.second.s] = IniVariable::String;
        else if(var.first == "ColorVariable")
            variableTypes[var.second.s] = IniVariable::Color;
        else if(var.first == "VectorVariable")
            variableTypes[var.second.s] = IniVariable::Vector;
        else if(var.first == "LineVariable")
            variableTypes[var.second.s] = IniVariable::Line;

        Log::debug("Variable type \"%s\" for variable \"%s\".", var.first, var.second.s);
    }

    for(auto& sub : object.subObjects){
        if(sub.second.args.size() > 0){
            Log::debug("SubObject name: \"%s\"", sub.second.args[0]);
            subTypes.emplace(sub.second.args[0], sub.second);
        }
    }
    /* TODO - Errors are possible. They should be handled. */
    return true;
}

}
