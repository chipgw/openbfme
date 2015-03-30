#include "bigreader.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstdio>
#include <cctype>

#include FILESYSTEM_HEADER
namespace fs = FILESYSTEM_NAMESPACE;

namespace OpenBFME {

uint32_t readUInt32(FILE* file){
    uint8_t val[4];
    fread(val, 1, 4, file);
    return val[0] << 24 | val[1] << 16 | val[2] << 8 | val[3];
}

string readString(FILE* file, uint32_t limit, char terminator = '\0'){
    string data;
    while(uint32_t(ftell(file)) < limit){
        character c = fgetc(file);
        if(c == '\0' || c == terminator || ferror(file) || feof(file)){
            break;
        }
        data += c;
    }
    return data;
}

BigArchive::BigArchive(const string &filename, BigFilesystem &fs) : archiveFilename(filename), file(nullptr), filesystem(fs) {
    std::replace(archiveFilename.begin(), archiveFilename.end(), '\\', '/');
}

BigArchive::~BigArchive(){
    close();
}

bool BigArchive::readHeader(){
    if (fs::is_directory(fs::path(archiveFilename))) {
        backend = Folder;

        for (fs::recursive_directory_iterator dir(archiveFilename), end; dir != end; ++dir) {
            const fs::path& currentPath = dir->path();
            if (fs::is_regular_file(currentPath)) {
                string filename = currentPath.generic_string();
                filename.erase(0, filename.find_first_of('/') + 1);
                uint32_t filesize = uint32_t(fs::file_size(currentPath));

                entries.emplace(*this, 0, filesize, filename);
                Log::debug("File path: \"%s\" length: %#08x", filename, filesize);
            }
        }

        return true;
    }

    backend = BigFile;

    if(!open()){
        return false;
    }

    character id[4] = {0};
    fread(id, 1, 4, file);

    if (id[0] != 'B' || id[1] != 'I' || id[2] != 'G' || (id[3] != '4' && id[3] != 'F')){
        close();
        return false;
    }

    fseek(file, 8, SEEK_SET);

    uint32_t fileCount = readUInt32(file);
    uint32_t headerEnd = readUInt32(file);

    Log::info("File Count: %u Header End: %#08x", fileCount, headerEnd);

    for(uint32_t f = 0; f < fileCount; ++f){
        uint32_t start = readUInt32(file);
        uint32_t end   = start + readUInt32(file);
        string path = readString(file, headerEnd);
        std::replace(path.begin(), path.end(), '\\', '/');

        entries.emplace(*this, start, end, path);

        Log::debug("File #%04d start: %#08x end: %#08x path: \"%s\"", f + 1, start, end, path);
    }

    if(uint32_t(ftell(file)) > headerEnd){
        Log::warning("Reading of file info passed through end of header.");
    }

    return true;
}

bool BigArchive::open(){
    if(file != nullptr){
        return true;
    }

    if(backend == BigFile){
        file = fopen(archiveFilename.c_str(), "rb");
    }

    return file != nullptr;
}

void BigArchive::close(){
    if(file != nullptr){
        fclose(file);
        file = nullptr;
    }
    currentEntry = nullptr;
}

bool BigArchive::openEntry(const BigEntry& entry) {
    entry.line = 0;
    /* Comments mode must be reenabled after opening a new file. */
    comments = false;

    switch (backend) {
    case BigFile:
        if (open()) {
            fseek(file, entry.start, SEEK_SET);
            currentEntry = &entry;
            return true;
        }
        break;
    case Folder:
        close();
        file = fopen((archiveFilename + '/' + entry.filename).c_str(), "rb");
        if (file != nullptr) {
            currentEntry = &entry;
            return true;
        }
        break;
    }
    return false;
}

const BigEntry* BigArchive::openFile(const string &filename){
    auto entry = entries.find(BigEntry(*this, 0, 0, filename));
    if(entry != entries.end() && openEntry(*entry)){
        return &(*entry);
    }

    return nullptr;
}

string BigArchive::getLine(const BigEntry &entry){
    if(!open() || eof(entry)){
        return "";
    }
    string str = readString(file, entry.end, '\n');

    if(str.back() == '\r'){
        str.pop_back();
    }

    string::size_type comment = std::min(str.find(';'), str.find("//"));
    if(comment != string::npos)
        str.erase(comment);

    entry.line++;

    return str;
}

string BigArchive::getWord(const BigEntry &entry){
    if(!open() || eof(entry)){
        return "";
    }

    bool isWrd = false;
    bool isStr = false;
    bool isNmb = false;
    bool isSym = false;

    string data;
    while(!eof(entry)){
        character c = fgetc(file);

        if(ferror(file) || feof(file)){
            break;
        }

        if(data.empty()){
            if(c == '\n'){
                /* return a newline character if it's the first non-space we run into. */
                entry.line++;
                return "\n";
            }
            if(std::isspace(c)){
                /* Ignore a space. */
                continue;
            }
            if(std::isalpha(c)){
                /* we have a word! */
                isWrd = true;
            }else if(std::isdigit(c)){
                /* we have a number! */
                isNmb = true;
            }else if(c == '"'){
                /* we have a string! */
                isStr = true;
            }else if(c == ';'){
                /* We have a comment! */
                getLine(entry);
                return "\n";
            }else{
                isSym = true;
            }
        }else{
            if(!isStr){
                if(c == ';'){
                    /* We have a comment! */
                    getLine(entry);
                    break;
                }else if(c == '/' && data.back() == '/'){
                    /* We have a comment! */
                    getLine(entry);

                    /* Is the comment all that was in the string? */
                    if(data.size() == 1)
                        return "\n";

                    /* Remove the other '/' character. */
                    data.pop_back();
                    break;
                }else if((std::isspace(c) || /* A space ends anything. */
                        /* Words break on non alpha-numeric characters other than underscores. */
                        (isWrd && !std::isalnum(c) && c != '_') ||
                        /* Numbers break on anything that isn't a digit or a period. */
                        (isNmb && !std::isdigit(c) && c != '.') ||
                        /* Symbols break on anything alpha-numeric. */
                        (isSym && std::isalnum(c)))){
                    ungetc(c, file);
                    break;
                }
            }else if(c == '"'){
                data += c;
                break;
            }
        }
        data += c;
    }

    return data;
}

bool BigArchive::seek(const BigEntry &entry, uint32_t pos){
    if(&entry != currentEntry){
        openEntry(entry);
    }

    pos += entry.start;
    if(pos < entry.end){
        fseek(file, pos, SEEK_SET);
        return true;
    }
    return false;
}

uint32_t BigArchive::tell(const BigEntry &entry){
    if(&entry != currentEntry){
        openEntry(entry);
    }

    uint32_t pos = ftell(file);
    if(pos < entry.start){
        return 0;
    }
    if(pos > entry.end){
        return entry.end - entry.start;
    }
    return pos - entry.start;
}

bool BigArchive::eof(const BigEntry &entry){
    uint32_t cpos = ftell(file);
    return cpos < entry.start || cpos >= entry.end;
}

bool BigArchive::extract(const string &filename, const string &directory, bool fullPath, bool ignore, bool overwrite){
    const BigEntry* entry = openFile(filename);

    if(entry == nullptr){
        return false;
    }

    string outfilename = filename;

    if(!fullPath){
        outfilename.erase(0, outfilename.find_last_of('/') + 1);
    }
    outfilename.insert(0, directory);
    fs::create_directories(fs::path(outfilename.substr(0, outfilename.find_last_of('/'))));

    if(fs::exists(fs::path(outfilename))){
        /* The option to ignore existing files takes precedence over overwriting them. */
        if(ignore){
            Log::info("Skipping existing file: \"%s\".", outfilename);

            return true;
        }else if(!overwrite){
            character c = 0;

            do{
                Log::info("Overwrite existing file \"%s\"? [Y/N]:", outfilename);

                c = std::tolower(std::getchar());

                /* Only take one letter input. */
                if(std::getchar() != '\n'){
                    c = 0;
                    while(std::getchar() != '\n') continue;
                }
            }while(c != 'n' && c != 'y');

            if(c == 'n'){
                Log::info("Skipping \"%s\".", outfilename);

                return true;
            }
        }
        Log::info("Overwriting \"%s\"...", outfilename);
    }else{
        Log::info("Extracting to \"%s\"...", outfilename);
    }

    FILE* out = fopen(outfilename.c_str(), "wb");

    if(out == nullptr){
        Log::error("Unable to create file \"%s\"!", outfilename);
        return false;
    }
    uint32_t length = entry->end - entry->start;
    uint8_t *buffer = new uint8_t[length];

    fread(buffer, 1, length, file);
    fwrite(buffer, 1, length, out);

    fclose(out);
    delete[] buffer;

    Log::info("File successfully extracted.");

    return true;
}

bool BigArchive::extractAll(const string &directory, bool ignore, bool overwrite){
    fs::create_directories(fs::path(directory));
    for(auto &entry : entries){
        if(!extract(entry.filename, directory, true, ignore, overwrite)){
            return false;
        }
    }
    return true;
}

}
