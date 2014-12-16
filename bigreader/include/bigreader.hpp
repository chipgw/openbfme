#pragma once

#include "types.hpp"
#include <set>
#include <cstdio>
#include <cstdint>
#include <list>

namespace OpenBFME {

class BigEntry;
class BigArchive;
class BigFilesystem;

class BigArchive{
public:
    enum Backend{
        BigFile,
        Folder
    };

private:
    std::set<BigEntry> entries;
    const BigEntry* currentEntry;
    FILE* file;
    string archiveFilename;
    BigFilesystem &filesystem;

    Backend backend;

public:
    EXPORT BigArchive(const string &filename, BigFilesystem &fs);
    EXPORT ~BigArchive();

    EXPORT bool readHeader();
    EXPORT bool open();
    EXPORT bool openEntry(const BigEntry& entry);
    EXPORT void close();

    EXPORT const BigEntry* openFile(const string &filename);

    EXPORT string getLine(const BigEntry &entry);
    EXPORT string getWord(const BigEntry &entry);
    EXPORT bool seek(const BigEntry &entry, uint32_t pos);
    EXPORT uint32_t tell(const BigEntry &entry);
    EXPORT bool eof(const BigEntry &entry);

    EXPORT bool extract(const string &filename, const string &directory, bool fullPath);
    EXPORT bool extractAll(const string &directory);

    EXPORT inline const string &getArchiveFilename() { return archiveFilename; }
    EXPORT inline const Backend &getBackend() { return backend; }
};


class BigEntry{
public:
    mutable uint32_t line;
    const string filename;
    const uint32_t start, end;
    BigArchive &archive;
    BigEntry(BigArchive &arch, uint32_t start, uint32_t end, string file);

    inline string getLine() const { return archive.getLine(*this); }
    inline string getWord() const { return archive.getWord(*this); }
    inline bool seek(uint32_t pos) const { return archive.seek(*this, pos); }
    inline uint32_t tell() const { return archive.tell(*this); }
    inline bool eof() const { return archive.eof(*this); }

};
inline bool operator <(const BigEntry& e1,const BigEntry& e2){ return e1.filename < e2.filename;}

class BigFilesystem{
    std::list<BigArchive> archives;

public:
    EXPORT BigArchive* mount(const string &filename, bool append);
    EXPORT bool unmount(const string &filename);
    EXPORT bool unmount(BigArchive* archive);

    EXPORT const BigEntry *openFile(const string &filename, const string &relativeTo = "");
};

}
