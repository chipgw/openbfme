#ifndef OPENBFME_BIGREADER_H
#define OPENBFME_BIGREADER_H

#include "types.h"
#include <set>
#include <deque>
#include <cstdio>
#include <cstdint>

namespace OpenBFME {

class BigEntry;
class BigArchive;
class BigFilesystem;

class BigArchive{
    std::set<BigEntry> entries;
    FILE* file;
    string archiveFilename;

public:
    BigArchive(const string &filename);
    ~BigArchive();

    bool readHeader();
    bool open();
    void close();

    const BigEntry* openFile(const string &filename);

    string getLine(const BigEntry &entry);
    string getWord(const BigEntry &entry);
    bool seek(const BigEntry &entry, uint32_t pos);
    uint32_t tell(const BigEntry &entry);
    bool eof(const BigEntry &entry);

    bool extract(const string &filename, const string &directory, bool fullPath);
    bool extractAll(const string &directory);

    inline const string &getArchiveFilename() { return archiveFilename; }
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
    std::deque<BigArchive> archives;

public:
    BigArchive* mount(const string &filename, bool append);
    bool unmount(const string &filename);
    bool unmount(BigArchive* archive);

    const BigEntry *openFile(const string &filename, const string &relativeTo = "");
};

}

#endif //OPENBFME_BIGREADER_H
