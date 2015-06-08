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

    /* Only one entry can be open at a time per archive, this remembers which one it is. */
    const BigEntry* currentEntry;

    /* For BigFile backend this is the archive file.
     * For Folder backends it is the file for the currentEntry. */
    FILE* file;

    /* File or folder path of the archive. */
    string archiveFilename;

    /* The current Backend of the file. Almost should be const, but isn't calculated in the constructor. */
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

    /* Find out what type of Backend to use and fills entries. Returns false on error. */
    bool readHeader();

    /* Search for and open an entry, nullptr if not found. */
    const BigEntry* openFile(const string &filename);
    
    /* Get a single character from a text file. */
    EXPORT character getChar(const BigEntry &entry);
    /* Put a single character back into a text file. */
    EXPORT void ungetChar(const BigEntry &entry, character c);

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
    /* Make a .big file out of a set of entries. */
    EXPORT static bool writeBig(const std::set<BigEntry>& entries, const string& filename);
    /* Make a .big file out of a folder. */
    EXPORT bool writeBig(const string& filename);

    inline const string &getArchiveFilename() { return archiveFilename; }
    inline const Backend &getBackend() { return backend; }
};

}
