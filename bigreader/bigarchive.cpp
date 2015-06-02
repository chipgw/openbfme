#include "bigentry.hpp"
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

void writeUInt32(FILE* file, uint32_t value){
    uint8_t val[] { uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8), uint8_t(value) };
    fwrite(val, 1, 4, file);
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

BigArchive::BigArchive(const string &filename) : archiveFilename(fs::path(filename).generic_string()), file(nullptr) { }

BigArchive::~BigArchive(){
    close();
}

bool BigArchive::readHeader(){
    if (fs::is_directory(fs::path(archiveFilename))) {
        backend = Folder;
        
        /* Make sure the archiveFilename ends with a '/' */
        if (archiveFilename.back() != '/')
            archiveFilename += '/';

        for (fs::recursive_directory_iterator dir(archiveFilename), end; dir != end; ++dir) {
            const fs::path& currentPath = dir->path();
            if (fs::is_regular_file(currentPath)) {
                string filename = currentPath.generic_string();

                /* Check to see if the filename contains the parent path, and if so remove it. */
                if (filename.compare(0, archiveFilename.size(), archiveFilename, 0, archiveFilename.size()) == 0)
                    filename.erase(0, archiveFilename.size());
                else
                    /* Otherwise fall back to removing the first element in the path. */
                    filename.erase(0, filename.find_first_of('/') + 1);

                uint32_t filesize = uint32_t(fs::file_size(currentPath));

                entries.emplace(*this, 0, filesize, filename);
                Log::debug("File path: \"%s\" length: %#08x", filename, filesize);
            }
        }

        return true;
    }

    backend = BigFile;

    if (!open())
        return false;

    /* The first 4 bytes shoule be either "BIG4" or "BIGF", quit if they aren't. */
    character id[4] = {0};
    fread(id, 1, 4, file);

    if (id[0] != 'B' || id[1] != 'I' || id[2] != 'G' || (id[3] != '4' && id[3] != 'F')) {
        Log::error("File \"%s\" is not a valid big file!");
        close();
        return false;
    }

    fseek(file, 8, SEEK_SET);

    uint32_t fileCount = readUInt32(file);
    uint32_t headerEnd = readUInt32(file);

    Log::info("File Count: %u Header End: %#08x", fileCount, headerEnd);

    /* Read the individual file information into entries. */
    for (uint32_t f = 0; f < fileCount; ++f) {
        uint32_t start = readUInt32(file);
        uint32_t end   = start + readUInt32(file);
        string path = readString(file, headerEnd);
        std::replace(path.begin(), path.end(), '\\', '/');

        entries.emplace(*this, start, end, path);

        Log::debug("File #%04d start: %#08x end: %#08x path: \"%s\"", f + 1, start, end, path);
    }

    if (uint32_t(ftell(file)) > headerEnd)
        Log::warning("Reading of file info passed through end of header.");

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
    entry.resetLineNumber();

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
    auto entry = std::find_if(entries.begin(), entries.end(), [&](const BigEntry& it){ return it.filename == filename; });

    if(entry != entries.end() && openEntry(*entry)){
        return &(*entry);
    }

    return nullptr;
}

character BigArchive::getChar(const BigEntry &entry){
    if (!open() || eof(entry)){
        return 0;
    }

    character ch = fgetc(file);

    if (ch == '\n'){
        entry.incrementLineNumber();
    }

    return ch;
}

void BigArchive::ungetChar(const BigEntry &entry, character c) {
    if (tell(entry) > 0) {
        ungetc(c, file);

        if (c == '\n') {
            entry.decrementLineNumber();
        }
    }
}

bool BigArchive::seek(const BigEntry &entry, uint32_t pos){
    if(&entry != currentEntry && !openEntry(entry))
        return false;

    /* Invalidate the stored current line number. */
    entry.invalidateLineNumber();

    pos += entry.start;
    if(pos < entry.end){
        fseek(file, pos, SEEK_SET);
        return true;
    }
    return false;
}

uint32_t BigArchive::tell(const BigEntry &entry){
    if (&entry != currentEntry && !openEntry(entry))
        return 0;

    uint32_t pos = ftell(file);
    if(pos < entry.start)
        return 0;

    if(pos > entry.end)
        return entry.end - entry.start;

    return pos - entry.start;
}

bool BigArchive::eof(const BigEntry &entry){
    uint32_t cpos = ftell(file);
    return cpos < entry.start || cpos >= entry.end;
}

bool BigArchive::extract(const BigEntry& entry, const string &directory, bool fullPath, bool ignore, bool overwrite){
    if(!openEntry(entry)){
        Log::error("Error opening entry \"%s\"!", entry.filename);
        return false;
    }

    fs::path path = entry.filename;

    path = fs::canonical(fs::path(directory)) / (fullPath ? path : path.filename());

    fs::create_directories(path.parent_path());

    if(fs::exists(path)){
        /* The option to ignore existing files takes precedence over overwriting them. */
        if(ignore){
            Log::info("Skipping existing file: \"%s\".", entry.filename);

            return true;
        }else if(!overwrite){
            character c = 0;

            while(c != 'n' && c != 'y'){
                Log::info("Overwrite existing file \"%s\"? [Y/N]:", path.generic_string());

                c = std::tolower(std::getchar());

                if(c == '\n') continue;

                /* Only take one letter input. */
                if(std::getchar() != '\n'){
                    c = 0;
                    while(std::getchar() != '\n') continue;
                }
            }

            if(c == 'n'){
                Log::info("Skipping \"%s\".", entry.filename);

                return true;
            }
        }
        Log::info("Overwriting \"%s\"...", entry.filename);
    }else{
        Log::info("Extracting to \"%s\"...", path.generic_string());
    }

    FILE* out = fopen(path.generic_string().c_str(), "wb");

    if(out == nullptr){
        Log::error("Unable to create file \"%s\"!", path.generic_string());
        return false;
    }
    uint32_t length = entry.end - entry.start;
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
        if(!extract(entry, directory, true, ignore, overwrite)){
            return false;
        }
    }
    return true;
}

bool BigArchive::writeBig(const std::set<BigEntry>& entries, const string& filename) {
    Log::info("Preparing to write %d files to \"%s\"", entries.size(), filename);

    /* 8 bytes for every entry + 24 at the start and end. */
    uint32_t headerLength = uint32_t(entries.size() * 8) + 20;

    /* Add the length of the filenames to headerLength. */
    for(auto &entry : entries){
        headerLength += uint32_t(entry.filename.size()) + 1;
    }

    Log::debug("Calculated header length: %#08x", headerLength);

    FILE* file = fopen(filename.c_str(), "wb");

    if (file == nullptr) {
        Log::error("Unable to open %s for writing!", filename);
        return false;
    }

    /* Either that or "BIG4". I'm not sure which... */
    fwrite("BIGF", 1, 4, file);

    writeUInt32(file, 0);

    /* General info about the file. */
    writeUInt32(file, uint32_t(entries.size()));
    writeUInt32(file, headerLength);

    /* Put the first file one byte after the end of the header. */
    uint32_t lastEnd = headerLength + 1;

    /* Write all the file information. */
    for(auto &entry : entries){
        uint32_t fileLength = entry.end - entry.start;
        writeUInt32(file, lastEnd);
        writeUInt32(file, fileLength);

        /* Write the filename and terminating null character. */
        fputs(entry.filename.c_str(), file);
        fputc('\0', file);

        /* So we know where the next file will be written. */
        lastEnd += fileLength;
    }

    /* What exactly is this? */
    fputs("L253", file);

    if(uint32_t(ftell(file)) > headerLength){
        Log::error("Calculated header length too short!");
    }

    /* One more byte until where we start the first file... */
    fputc('\0', file);

    /* Write all the files. */
    for(auto &entry : entries){
        Log::info("Writing file \"%s\".", entry.filename);
        entry.seek(0);

        for(character c; !entry.eof();){
            c = entry.getChar();
            fwrite(&c, sizeof(character), 1, file);
        }
    }

    fclose(file);

    Log::info("Finished writing to \"%s\".", filename);

    return true;
}

bool BigArchive::writeBig(const string& filename) {
    /* We only do this on a folder backend, because why would you do it on a .big? */
    if (backend == Folder) {
        return writeBig(entries, filename);
    }
    return false;
}

}
