#include "log.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include <algorithm>

namespace OpenBFME {

namespace {

/* Creates the IniType used to parse IniType definitions. Recursivly creates identical subtypes recursion layers deep. */
IniType makeDefinitionType(integer recursion) {
    IniType type;

    type.variableTypes.emplace("Bool",      IniVariable::String);
    type.variableTypes.emplace("Integer",   IniVariable::String);
    type.variableTypes.emplace("Decimal",   IniVariable::String);
    type.variableTypes.emplace("Percent",   IniVariable::String);
    type.variableTypes.emplace("String",    IniVariable::String);
    type.variableTypes.emplace("Color",     IniVariable::String);
    type.variableTypes.emplace("Vector",    IniVariable::String);
    type.variableTypes.emplace("Line",      IniVariable::String);

    if(recursion > 0)
        type.subTypes.emplace("SubType", makeDefinitionType(recursion - 1));

    return type;
}

/* Create an IniType to parse with, using 16 layers of recursion. */
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

bool IniType::loadFromIni(const IniObject& object) {
    for(auto& var : object.variables){
        if(var.first == "Bool")
            variableTypes[var.second.s] = IniVariable::Bool;
        else if(var.first == "Integer")
            variableTypes[var.second.s] = IniVariable::Integer;
        else if(var.first == "Decimal")
            variableTypes[var.second.s] = IniVariable::Decimal;
        else if(var.first == "Percent")
            variableTypes[var.second.s] = IniVariable::Percent;
        else if(var.first == "String")
            variableTypes[var.second.s] = IniVariable::String;
        else if(var.first == "Color")
            variableTypes[var.second.s] = IniVariable::Color;
        else if(var.first == "Vector")
            variableTypes[var.second.s] = IniVariable::Vector;
        else if(var.first == "Line")
            variableTypes[var.second.s] = IniVariable::Line;

        Log::debug("Variable type \"%s\" for variable \"%s\".", var.first, var.second.s);
    }

    for (auto& sub : object.subObjects) {
        if (sub.second.args.size() > 0) {
            Log::debug("SubObject name: \"%s\"", sub.second.args[0]);
            subTypes.emplace(sub.second.args[0], sub.second);
        }
    }
    /* TODO - Errors are possible. They should be handled. */
    return true;
}

}
