#pragma once

#include "bigarchive.hpp"

namespace OpenBFME {

class BigEntry{
    /* The current line of a text file. Negative if invalid. */
    mutable integer line;

public:
    /* Name of the file relative to archive root. */
    const string filename;
    /* The start and end address of a file inside a .big archive,
     * or 0 and the length of the file for a folder backend. */
    const uint32_t start, end;

    /* The archive that contains this file. */
    BigArchive &archive;
    BigEntry(BigArchive &arch, uint32_t start, uint32_t end, string file) : archive(arch), start(start), end(end), filename(file) {}

    /* Wrap IO functions from BigArchive for easy access. */
    inline string getLine(bool checkComments) const { return archive.getLine(*this, checkComments); }
    EXPORT string getWord() const;
    inline character getChar() const { return archive.getChar(*this); }
    inline bool seek(uint32_t pos) const { return archive.seek(*this, pos); }
    inline uint32_t tell() const { return archive.tell(*this); }
    inline bool eof() const { return archive.eof(*this); }

    /* Get the line number of the current location in the file. */
    EXPORT integer getLineNumber() const;

    /* For use by BigArchive. */
    integer incrementLineNumber() const;
    integer decrementLineNumber() const;
    inline void resetLineNumber() const { line = 0; }
    inline void invalidateLineNumber() const { line = -1; }
};

inline bool operator <(const BigEntry& e1,const BigEntry& e2){ return e1.filename < e2.filename;}

}
