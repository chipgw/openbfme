#include "bigreader.hpp"
#include "log.hpp"
#include <algorithm>
#include <cstdio>
#include <cctype>

#ifdef STD_FILESYSTEM
#include <filesystem>
/* TODO - this namespace is temporary, as this functionality isn't final yet. */
namespace fs = std::tr2::sys;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

namespace OpenBFME {

// TODO - do a better job of this... (maybe use SDL?)
uint32_t readUInt32(FILE* file){
    uint8_t val[4];
    fread(val, 1, 4, file);
    return val[0] << 24 | val[1] << 16 | val[2] << 8 | val[3];
}

string readString(FILE* file, uint32_t limit, char terminator = '\0'){
    string data;
    char c;
    do{
        c = fgetc(file);
        if(c == '\0' || c == terminator || ferror(file) || feof(file)){
            break;
        }
        data += c;
    }while(ftell(file) < limit);
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
                auto entry = entries.emplace(*this, 0, fs::file_size(currentPath), currentPath.string().substr(currentPath.string().find_first_of('/') + 1));
                Log::debug("File path: \"%s\" length: %#08x", entry.first->filename, entry.first->end);
            }
        }

        return true;
    }

    if(!open()){
        return false;
    }

    char id[4] = {0};
    fread(id, 1, 4, file);

    if (id[0] != 'B' || id[1] != 'I' || id[2] != 'G' || (id[3] != '4' && id[3] != 'F')){
        close();
        return false;
    }

    backend = BigFile;

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

    if(ftell(file) > headerEnd){
        Log::warning("Reading of file info passed through end of header.");
    }

    return true;
}

bool BigArchive::open(){
    if(file != nullptr){
        return true;
    }

    file = fopen(archiveFilename.c_str(), "rb");

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

    switch (backend) {
    case BigFile:
        if (open()) {
            fseek(file, entry.start, SEEK_SET);
            currentEntry = &entry;
            return true;
        }
        break;
    case Folder:
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
    if(entry != entries.end()){
        if(openEntry(*entry)){
            return &(*entry);
        }
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
    char c;
    do{
        c = fgetc(file);

        if(ferror(file) || feof(file)){
            break;
        }

        if(data.empty()){
            if(c == '\n'){
                // return a newline character if it's the first non-space we run into.
                entry.line++;
                return "\n";
            }
            if(std::isspace(c)){
                // Ignore a space.
                continue;
            }
            if(std::isalpha(c)){
                // we have a word!
                isWrd = true;
            }else if(std::isdigit(c)){
                // we have a number!
                isNmb = true;
            }else if(c == '"'){
                // we have a string!
                isStr = true;
            }else{
                // TODO - filter this further.
                isSym = true;
            }
        }else{
            if(!isStr && (std::isspace(c) ||
                    (isWrd && !std::isalnum(c) && c != '_') ||
                    (isNmb && !std::isdigit(c) && c != '.') ||
                    (isSym && std::isalnum(c)))){
                ungetc(c, file);
                break;
            }
            if(isStr && c == '"'){
                data += c;
                break;
            }
        }
        data += c;
    }while(ftell(file) < entry.end);

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

bool BigArchive::extract(const string &filename, const string &directory, bool fullPath){
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

    Log::info("Extracting to \"%s\"...", outfilename);

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

bool BigArchive::extractAll(const string &directory){
    fs::create_directories(fs::path(directory));
    for(auto &entry : entries){
        if(!extract(entry.filename, directory, true)){
            return false;
        }
    }
    return true;
}

}
