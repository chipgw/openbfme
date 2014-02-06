#include "iniparser.h"
#include "log.h"
#include <algorithm>
#include <functional>
#include <locale>

namespace OpenBFME {

IniParser::IniParser(BigFilesystem &filesys) : filesystem(filesys) {}

void IniParser::parse(const BigEntry &file, std::function<void (std::string, std::string)> set){
    while(!file.eof()){
        std::string word = file.getWord();

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
                    Log::error("%s:%d: expected a string after #include!", file.filename.c_str(), file.line);
                    // TODO - figure out what happens on an error...
                    continue;
                }
                word.erase(0, 1);
                word.erase(word.length() - 1, 1);

                uint32_t pos = file.tell();

                const BigEntry* includeFile = filesystem.openFile(word, file.filename);

                if(includeFile != nullptr){
                    parse(*includeFile, set);
                }else{
                    Log::error("Unable to open included file \"%s\"", word.c_str());
                }

                file.seek(pos);

                word = file.getWord();
                if(word != "\n"){
                    Log::error("%s:%d: expected newline after #include!", file.filename.c_str(), file.line);
                }
            }else if(word == "define"){
                // TODO - store defined value.

                auto macroName = file.getWord();

                if(macroName == "\n"){
                    Log::error("%s:%d: expected macro name after #define!", file.filename.c_str(), file.line);
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
        }else{
            set(word.c_str(), "");
            Log::info("%s:%d: %s", file.filename.c_str(), file.line, word.c_str());
        }
    }
}

}
