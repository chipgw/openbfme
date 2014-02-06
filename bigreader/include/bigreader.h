#ifndef OPENBFME_BIGREADER_H
#define OPENBFME_BIGREADER_H

#include <set>
#include <deque>
#include <string>
#include <cstdio>
#include <cstdint>

namespace OpenBFME {

class BigEntry;
class BigArchive;
class BigFilesystem;

class BigArchive{
    std::set<BigEntry> entries;
    FILE* file;
    std::string archiveFilename;

public:
    BigArchive(const std::string &filename);
    ~BigArchive();

    bool readHeader();
    bool open();
    void close();

    const BigEntry* openFile(const std::string &filename);

    std::string getLine(const BigEntry &entry);
    bool eof(const BigEntry &entry);

    bool extract(const std::string &filename, const std::string &directory, bool fullPath);
    bool extractAll(const std::string &directory);

    inline const std::string &getArchiveFilename() { return archiveFilename; }
};


class BigEntry{
    friend class BigArchive;
    friend bool operator <(const BigEntry& e1,const BigEntry& e2);

protected:
    const std::string filename;
    const uint32_t start, end;
    BigArchive &archive;
    BigEntry(BigArchive &arch, uint32_t start, uint32_t end, std::string file);

public:
    inline std::string getLine() const { return archive.getLine(*this); }
    inline bool eof() const { return archive.eof(*this); }
};
inline bool operator <(const BigEntry& e1,const BigEntry& e2){ return e1.filename < e2.filename;}

class BigFilesystem{
    std::deque<BigArchive> archives;

public:
    BigArchive* mount(const std::string &filename, bool append);
    bool unmount(const std::string &filename);
    bool unmount(BigArchive* archive);

    const BigEntry *openFile(const std::string &filename, const std::string &relativeTo = "");
};

}

#endif //OPENBFME_BIGREADER_H
