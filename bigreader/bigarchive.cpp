#include "bigreader.h"
#include "log.h"
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>

namespace OpenBFME {

// TODO - do a better job of this... (maybe use SDL?)
uint32_t readUInt32(FILE* file){
    uint8_t val[4];
    fread(val, 1, 4, file);
    return val[0] << 24 | val[1] << 16 | val[2] << 8 | val[3];
}

std::string readString(FILE* file, uint32_t limit, char terminator = '\0'){
    std::string data;
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

bool mkPath(std::string dir){
    for(char &p : dir){
        if(p == '/' || p == '\\'){
            p = '\0';
#ifdef _WIN32
            mkdir(dir.c_str());
#else
            mkdir(dir.c_str(), 0777);
#endif
            p = '/';
        }
    }
    // TODO - error handling/reporting.
    return true;
}

BigArchive::BigArchive(const std::string &filename) : archiveFilename(filename), file(nullptr) {
    std::replace(archiveFilename.begin(), archiveFilename.end(), '\\', '/');
}

BigArchive::~BigArchive(){
    close();
}

bool BigArchive::readHeader(){
    if(!open()){
        return false;
    }

    char id[4] = {0};
    fread(id, 1, 4, file);

    if (id[0] != 'B' || id[1] != 'I' || id[2] != 'G' || (id[3] != '4' && id[3] != 'F')){
        close();
        return false;
    }

    fseek(file, 8, SEEK_SET);

    uint32_t fileCount = readUInt32(file);
    uint32_t headerEnd = readUInt32(file);

    Log::info("File Count: %u Header End: 0x%08x", fileCount, headerEnd);

    for(uint32_t f = 0; f < fileCount; ++f){
        uint32_t start = readUInt32(file);
        uint32_t end   = start + readUInt32(file);
        std::string path = readString(file, headerEnd);
        std::replace(path.begin(), path.end(), '\\', '/');

        entries.emplace(BigEntry(*this, start, end, path));

        Log::info("File #%04d start: 0x%08x end: 0x%08x path: \"%s\"", f + 1, start, end, path.c_str());
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
}

const BigEntry* BigArchive::openFile(const std::string &filename){
    if(open()){
        auto entry = entries.find(BigEntry(*this, 0, 0, filename));
        if(entry != entries.end()){
            fseek(file, entry->start, SEEK_SET);
            return &(*entry);
        }
    }

    return nullptr;
}

std::string BigArchive::getLine(const BigEntry &entry){
    if(!open() || eof(entry)){
        return "";
    }
    std::string str = readString(file, entry.end, '\n');

    if(str.back() == '\r'){
        str.pop_back();
    }

    return str;
}

bool BigArchive::eof(const BigEntry &entry){
    uint32_t cpos = ftell(file);
    return cpos < entry.start || cpos >= entry.end;
}

bool BigArchive::extract(const std::string &filename, const std::string &directory, bool fullPath){
    const BigEntry* entry = openFile(filename);

    if(entry == nullptr){
        return false;
    }

    std::string outfilename = filename;

    if(!fullPath){
        outfilename.erase(0, outfilename.find_last_of('/') + 1);
    }
    outfilename.insert(0, directory);
    mkPath(outfilename);

    Log::info("Extracting to \"%s\"...", outfilename.c_str());

    FILE* out = fopen(outfilename.c_str(), "wb");

    if(out == nullptr){
        Log::error("Unable to create file \"%s\"!", outfilename.c_str());
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

bool BigArchive::extractAll(const std::string &directory){
    mkPath(directory);
    for(auto &entry : entries){
        if(!extract(entry.filename, directory, true)){
            return false;
        }
    }
    return true;
}

}
