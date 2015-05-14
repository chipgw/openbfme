#include "inivariable.hpp"
#include "initype.hpp"
#include "iniobject.hpp"
#include "iniparser.hpp"
#include "bigentry.hpp"
#include "bigfilesystem.hpp"
#include "log.hpp"
#include <algorithm>
#include <locale>

namespace OpenBFME {

void IniParser::parse(const BigEntry &file, IniObject &object){
    while(!file.eof()){
        string word = file.getWord();

        if(word == "\n"){
            continue;
        }

        if(word == "#"){
            parseMacro(file, object);
        }else if(object.type.breaks && word == object.type.breakWord){
            break;
        }else if(object.type.subTypes.count(word)){
            auto obj = object.subObjects.emplace(word, object.type.subTypes.at(word));

            string arg;
            while(arg != "\n"){
                if(!arg.empty()){
                    obj->second.args.push_back(arg);
                }
                arg = file.getWord();
            }

            Log::debug("Created object of type: \"%s\"", word);

            parse(file, obj->second);
        }else if(object.type.variableTypes.count(word)){
            auto var = object.variables.emplace(word, object.type.variableTypes.at(word));

            /* If there was an error parsing remove item. */
            if(!parseVariable(file, var->second, word))
                object.variables.erase(var);
        }
    }
}

bool IniParser::parseMacro(const BigEntry &file, IniObject &object){
    string word = file.getWord();

    if(word == "include"){
        word = file.getWord();

        /* Sure, the size probably should be a lot longer than 2, but 2 is all that matters here. */
        if(word.size() < 2 || word.front() != '"' || word.back() != '"'){
            Log::error("%s:%d: expected a string after #include!", file.filename, file.line);
            return false;
        }
        /* Trim the quotes. */
        word.erase(0, 1);
        word.pop_back();

        /* So we know where to resume this file from when we're done with the other one. */
        uint32_t pos = file.tell();

        const BigEntry* includeFile = BigFilesystem::openFile(word, file.filename);

        if(includeFile != nullptr){
            parse(*includeFile, object);
        }else{
            Log::error("%s:%d: Unable to open included file \"%s\"", file.filename, file.line, word);
        }

        /* Go back to where we were in this file. */
        file.seek(pos);

        word = file.getWord();
        if(word != "\n"){
            Log::warning("%s:%d: Expected newline after #include, got %s!", file.filename, file.line, word);
        }
        return true;
    }else if(word == "define"){
        auto macroName = file.getWord();

        if(macroName == "\n"){
            Log::error("%s:%d: Expected macro name after #define!", file.filename, file.line);
            return false;
        }

        auto macroValue = file.getLine(true);

        if(macros.count(macroName) > 0){
            /* TODO - I'm not sure if using the original value is the correct behavior,
             * but with emplace() it happens anyway, so this makes it clear what's going on.
             * If this turns out to be wrong I'll fix it. */
            Log::warning("%s:%d: Macro with name \"%s\" already exists! Using original value of \"%s\".",
                         file.filename, file.line, macroName, macros[macroName]);
        }else if(macroValue == "\n"){
            macros.emplace(macroName, "");
            Log::debug("Added macro: %s", macroName);
        }else{
            macros.emplace(macroName, macroValue);
            Log::debug("Added macro: %s value: %s", macroName, macroValue);
        }
        return true;
    }

    return false;
}

bool IniParser::parseVariable(const BigEntry &file, IniVariable& var, const std::string& name){
    /* This should be "=" most of the time, but IDK if it always is. Ignore it for now... */
    file.getWord();

    switch(var.type){
    case IniVariable::Bool:
        return parseBool(file, var, name);
    case IniVariable::Integer:
        return parseInteger(file, var, name);
    case IniVariable::Decimal:
        return parseDecimal(file, var, name);
    case IniVariable::Percent:
        return parseDecimal(file, var, name, 0.01f);
    case IniVariable::String:
        /* TODO - IDK if this should trim quotes or not... */
        var.s = file.getWord();
        Log::debug("Added variable: \"%s\" of type: \"String\" value: %s", name, var.s);
        return true;
    case IniVariable::Color:
        return parseVector(file, var, name, 1.f / 255.f);
    case IniVariable::Vector:
        return parseVector(file, var, name);
    case IniVariable::Line:
        var.s = file.getLine(true);
        Log::debug("Added variable: \"%s\" of type: \"Line\" value: \"%s\"", name, var.s);
        return true;
    }

    return false;
}

string IniParser::getVariableWord(const BigEntry &file){
    string word = file.getWord();
    return (macros.count(word) > 0) ? macros[word] : word;
}

bool IniParser::parseBool(const BigEntry &file, IniVariable &var, const std::string &name){
    string value = getVariableWord(file);
    if(value == "Yes"){
        var.b = true;
    }else if(value == "No"){
        var.b = false;
    }else{
        Log::error("%s:%d: Expected \"Yes\" or \"No\" value after variable \"%s\", got \"%s\"!", file.filename.c_str(), file.line, name, value);
        return false;
    }
    Log::debug("Added variable: \"%s\" of type: \"Bool\" value: %s", name, value);
    return true;
}

bool IniParser::parseInteger(const BigEntry &file, IniVariable &var, const std::string& name, integer mult){
    string value = getVariableWord(file);

    if(value == "-"){
        value = getVariableWord(file);
        mult *= -1;
    }

    try{
        var.i = std::stoi(value) * mult;
    }catch(...){
        Log::error("%s:%d: Expected integer value after variable \"%s\", got \"%s\"!", file.filename.c_str(), file.line, name, value);
        return false;
    }

    Log::debug("Added variable: \"%s\" of type: \"Integer\" value: %i", name, var.i);
    return true;
}

bool IniParser::parseDecimal(const BigEntry &file, IniVariable &var, const std::string& name, decimal mult){
    string value = getVariableWord(file);

    if(value == "-"){
        value = getVariableWord(file);
        mult *= -1.0f;
    }

    try{
        var.d = std::stof(value) * mult;
    }catch(...){
        Log::error("%s:%d: Expected decimal value after variable \"%s\", got \"%s\"!", file.filename.c_str(), file.line, name, value);
        return false;
    }

    Log::debug("Added variable: \"%s\" of type: \"Decimal\" value: %f", name, var.d);
    return true;
}

bool IniParser::parseVector(const BigEntry &file, IniVariable &var, const std::string& name, decimal mult){
    for(string component = file.getWord(); component != "\n"; component = file.getWord()){
        float val = 1.0f;

        file.getWord(); /* <- This should be a colon, just ignore it. */
        string valStr = file.getWord();

        if(valStr == "-"){
            valStr = file.getWord();
            val = -1.0f;
        }

        try{
            /* multiply by either 1 or -1 based on whether or not there was a negative symbol beforehand. */
            val *= std::stof(valStr) * mult;
        }catch(...){
            Log::error("%s:%d: Expected decimal value after vector component, got \"%s\"!", file.filename.c_str(), file.line, valStr);
            return false;
        }

        if(component == "X" || component == "R"){
            var.v.x = val;
        }else if(component == "Y" || component == "G"){
            var.v.y = val;
        }else if(component == "Z" || component == "B"){
            var.v.z = val;
        }else{
            Log::error("%s:%d: Expected a vector component letter, got \"%s\"!", file.filename.c_str(), file.line, component);
            return false;
        }
    }

    Log::debug("Added variable: \"%s\" of type: \"Vector\" %f %f %f", name, var.v.x, var.v.y, var.v.z);
    return true;
}

}
