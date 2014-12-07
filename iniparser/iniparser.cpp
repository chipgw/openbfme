#include "iniparser.hpp"
#include "log.hpp"
#include <algorithm>
#include <locale>

namespace OpenBFME {

IniParser::IniParser(BigFilesystem &filesys) : filesystem(filesys) {}

void IniParser::parse(const BigEntry &file, IniObject object){
    while(!file.eof()){
        string word = file.getWord();

        if((word.size() > 1 && word[0] == '/' && word[1] == '/') || word[0] == ';'){
            file.getLine();
            continue;
        }
        if(word == "\n"){
            continue;
        }

        if(word == "#"){
            word = file.getWord();

            if(word == "include"){
                word = file.getWord();

                if(word[0] != '"'){
                    Log::error("%s:%d: expected a string after #include!", file.filename, file.line);
                    // TODO - figure out what happens on an error...
                    continue;
                }
                word.erase(0, 1);
                word.erase(word.length() - 1, 1);

                uint32_t pos = file.tell();

                const BigEntry* includeFile = filesystem.openFile(word, file.filename);

                if(includeFile != nullptr){
                    parse(*includeFile, object);
                }else{
                    Log::error("Unable to open included file \"%s\"", word);
                }

                file.seek(pos);

                word = file.getWord();
                if(word != "\n"){
                    Log::error("%s:%d: expected newline after #include!", file.filename, file.line);
                }
            }else if(word == "define"){
                auto macroName = file.getWord();

                if(macroName == "\n"){
                    Log::error("%s:%d: expected macro name after #define!", file.filename, file.line);
                    // TODO - figure out what happens on an error...
                    continue;
                }

                auto macroValue = file.getWord();

                if(macroValue == "\n"){
                    macros.emplace(macroName, "");
                }else{
                    macros.emplace(macroName, macroValue);
                }
            }
        }else if(object.type.breaks && word == object.type.breakWord){
            break;
        }else if(object.type.subTypes.count(word)){
            object.subObjects.emplace(word, object.type.subTypes.at(word));

            string arg;
            while(arg != "\n"){
                if(!arg.empty()){
                    object.subObjects.at(word).args.push_back(arg);
                }
                arg = file.getWord();
            }

            Log::info("Created object of type: \"%s\"", word);

            parse(file, object.subObjects.at(word));
        }else if(object.type.variableTypes.count(word)){
            object.variables[word].type = object.type.variableTypes.at(word);

            /* TODO - Implement. */
            switch(object.variables[word].type){
            case IniVariable::Integer:
                Log::info("added variable: \"%s\" of type: \"Integer\"", word);
                break;
            case IniVariable::Decimal:
                Log::info("added variable: \"%s\" of type: \"Decimal\"", word);
                break;
            case IniVariable::String:
                Log::info("added variable: \"%s\" of type: \"String\"", word);
                break;
            }
        }
    }
}

}
