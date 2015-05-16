#include "bigentry.hpp"

namespace OpenBFME {

integer BigEntry::getLineNumber() const {
    if (line < 0) {
        /* Go back and count lines in the file. */
        uint32_t pos = tell();
        seek(0);
        line = 0;

        for (uint32_t i = 0; i < pos; ++i){
            getChar();
        }
    }

    return line;
}

integer BigEntry::incrementLineNumber() const {
    if (line >= 0) {
        return ++line;
    }
    /* Delegate to getLineNumber to calculate the current line. */
    return getLineNumber();
}

}
