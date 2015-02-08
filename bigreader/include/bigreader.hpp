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

    /* Open the archive file in a BigFile backend.
     * If called with a folder backend returns true if an entry is already open. */
    bool open();
    /* Close any open file. */
    void close();
    /* Open an entry based on backend. */
    bool openEntry(const BigEntry& entry);

public:
    EXPORT BigArchive(const string &filename, BigFilesystem &fs);
    EXPORT ~BigArchive();

    bool readHeader();
    const BigEntry* openFile(const string &filename);

    /* Get a line from a text file, does not include newline character. */
    EXPORT string getLine(const BigEntry &entry);

    /* Get a */
    EXPORT string getWord(const BigEntry &entry);

    /* Seek to a position inside the entry. */
    EXPORT bool seek(const BigEntry &entry, uint32_t pos);
    /* Return the current position inside the entry. */
    EXPORT uint32_t tell(const BigEntry &entry);
    /* Returns true if the current position is not within the file bounds. */
    EXPORT bool eof(const BigEntry &entry);

    /* Extract a file to the specified directory. */
    EXPORT bool extract(const string &filename, const string &directory, bool fullPath);
    /* Extract all entries into given directory. */
    EXPORT bool extractAll(const string &directory);

    inline const string &getArchiveFilename() { return archiveFilename; }
    inline const Backend &getBackend() { return backend; }
};

class BigEntry{
public:
    /* The current line of a text file. */
    mutable uint32_t line;
    /* Name of the file relative to archive root. */
    const string filename;
    /* The start and end address of a file inside a .big archive,
     * or 0 and the length of the file for a folder backend. */
    const uint32_t start, end;

    /* The archive that contains this file. */
    BigArchive &archive;
    BigEntry(BigArchive &arch, uint32_t start, uint32_t end, string file) : archive(arch), start(start), end(end), filename(file) {}

    /* Wrap IO functions from BigArchive for easy access. */
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
