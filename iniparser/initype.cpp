#include "log.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigfilesystem.hpp"
#include <tinyxml.h>
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
    loadFromXML(filename);
}

IniType::IniType(const string& filename, BigFilesystem &big) {
    loadFromIni(filename, big);
}

IniType::IniType(const IniObject& object) {
    loadFromIni(object);
}

bool IniType::loadFromIni(const string& filename, BigFilesystem& big) {
    IniParser ini(big);
    if(const BigEntry* file = big.openFile(filename)){
        IniObject root(IniDefinitionType);
        ini.parse(*file, root);

        loadFromIni(root);
    }else{
        Log::warning("Unable to open file!");
    }
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
}

bool IniType::loadFromXML(TiXmlElement* element, const string& filename) {
    for(; element != nullptr; element = element->NextSiblingElement()){
        if(element->ValueStr() == "variable"){
            string varName, varType;

            if(element->QueryStringAttribute("name", &varName) == TIXML_SUCCESS &&
               element->QueryStringAttribute("type", &varType) == TIXML_SUCCESS && !varName.empty()){
                std::transform(varType.begin(), varType.end(), varType.begin(), ::tolower);

                if(varType == "bool")
                    variableTypes[varName] = IniVariable::Bool;
                else if(varType == "integer")
                    variableTypes[varName] = IniVariable::Integer;
                else if(varType == "decimal")
                    variableTypes[varName] = IniVariable::Decimal;
                else if(varType == "percent")
                    variableTypes[varName] = IniVariable::Percent;
                else if(varType == "string")
                    variableTypes[varName] = IniVariable::String;
                else if(varType == "color")
                    variableTypes[varName] = IniVariable::Color;
                else if(varType == "vector")
                    variableTypes[varName] = IniVariable::Vector;
                else if(varType == "line")
                    variableTypes[varName] = IniVariable::Line;
                else
                    Log::error("%s:%i Invalid type \"%s\" for variable \"%s\"!", filename, element->Row(), varType, varName);

                Log::debug("Variable type \"%s\" for variable \"%s\".", varType, varName);
            }
        }else if(element->ValueStr() == "subtype"){
            string name, file;

            if(element->QueryStringAttribute("name", &name) == TIXML_SUCCESS && !name.empty()){
                if(element->QueryStringAttribute("file", &file) == TIXML_SUCCESS)
                    subTypes[name].loadFromXML(file);
                else
                    subTypes[name].loadFromXML(element->FirstChildElement(), filename);
            }
        }
    }

    return true;
}

bool IniType::loadFromXML(const string& filename) {
    TiXmlDocument doc(filename);

    if(!doc.LoadFile()){
        Log::error("Unable to load file \"%s\"! %s", filename, doc.ErrorDesc());
        return false;
    }

    TiXmlElement* root = doc.FirstChildElement("openbfme-initype");
    if(root == nullptr){
        Log::error("\"%s\" is not a valid ini type definition file!", filename);
        return false;
    }

    loadFromXML(root->FirstChildElement(), filename);

    return true;
}

}
