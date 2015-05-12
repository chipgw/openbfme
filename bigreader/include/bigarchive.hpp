#pragma once

#include "types.hpp"
#include <set>
#include <cstdio>

namespace OpenBFME {

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

    Backend backend;

    /* Open the archive file in a BigFile backend.
     * If called with a folder backend returns true if an entry is already open. */
    bool open();
    /* Close any open file. */
    void close();
    /* Open an entry based on backend. */
    bool openEntry(const BigEntry& entry);

public:
    BigArchive(const string &filename);
    ~BigArchive();

    bool readHeader();
    const BigEntry* openFile(const string &filename);

    /* Get a line from a text file, does not include newline character. */
    EXPORT string getLine(const BigEntry &entry, bool checkComments);

    /* Get a */
    EXPORT string getWord(const BigEntry &entry);

    /* Seek to a position inside the entry. */
    EXPORT bool seek(const BigEntry &entry, uint32_t pos);
    /* Return the current position inside the entry. */
    EXPORT uint32_t tell(const BigEntry &entry);
    /* Returns true if the current position is not within the file bounds. */
    EXPORT bool eof(const BigEntry &entry);

    /* Extract a file to the specified directory. */
    EXPORT bool extract(const BigEntry &entry, const string &directory, bool fullPath, bool ignore, bool overwrite);
    /* Extract all entries into given directory. */
    EXPORT bool extractAll(const string &directory, bool ignore, bool overwrite);

    inline const string &getArchiveFilename() { return archiveFilename; }
    inline const Backend &getBackend() { return backend; }
};

}
