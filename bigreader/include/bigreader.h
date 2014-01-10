#ifndef OPENBFME_BIGREADER_H
#define OPENBFME_BIGREADER_H

#include <map>
#include <deque>
#include <string>
#include <cstdio>

namespace OpenBFME {

class BigEntry;
class BigArchive;
class BigFilesystem;

class BigArchive{
    friend class BigFilesystem;
protected:
    std::map<std::string, BigEntry> entries;
    FILE* file;
    std::string archiveFilename;

public:
    BigArchive(const std::string &filename);
    ~BigArchive();

    bool readHeader();
    bool open();
    void close();

    BigEntry* openFile(const std::string &filename);

    std::string getLine(BigEntry &entry);
    bool eof(BigEntry &entry);

    bool extract(const std::string &filename, const std::string &directory, bool fullPath);
    bool extractAll(const std::string &directory);
};


class BigEntry{
    friend class BigArchive;
protected:
    uint32_t start, end;
    BigArchive &archive;
    BigEntry(BigArchive &arch, uint32_t start, uint32_t end);

public:
    inline std::string getLine() { return archive.getLine(*this); }
    inline bool eof() { return archive.eof(*this); }
};

class BigFilesystem{
    std::deque<BigArchive> archives;

public:
    BigArchive* mount(const std::string &filename, bool append);
    bool unmount(const std::string &filename);
    bool unmount(BigArchive* archive);

    BigEntry* openFile(const std::string &filename);
};

}

#endif //OPENBFME_BIGREADER_H
