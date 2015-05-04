#include "log.hpp"
#include "initype.hpp"
#include <tinyxml.h>
#include <algorithm>

namespace OpenBFME {

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
