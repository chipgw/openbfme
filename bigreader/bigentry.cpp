#include "bigentry.hpp"
#include <cctype>
#include <algorithm>

namespace OpenBFME {

string BigEntry::getLine(bool checkComments) const {
    string line;

    for (character c = getChar(); c != 0 && c != '\n' && !eof(); c = getChar())
        line += c;

    /* We don't want the carriage return that might have been before the newline. */
    if (line.back() == '\r')
        line.pop_back();

    if (checkComments) {
        string::size_type comment = std::min(line.find(';'), line.find("//"));
        if (comment != string::npos)
            line.erase(comment);
    }

    return line;
}

string BigEntry::getWord() const {
    if (eof()) return "";

    bool isWrd = false;
    bool isStr = false;
    bool isNmb = false;
    bool isSym = false;

    string data;
    for (character c = getChar(); !eof(); c = getChar()) {
        if (data.empty()) {
            if(c == '\n')
                /* return a newline character if it's the first thing we run into. */
                return "\n";

            if(std::isspace(c)) {
                /* Ignore a space. */
                continue;
            }
            if (std::isalpha(c)) {
                /* we have a word! */
                isWrd = true;
            } else if (std::isdigit(c)) {
                /* we have a number! */
                isNmb = true;
            } else if (c == '"') {
                /* we have a string! */
                isStr = true;
            } else if (c == ';'){
                /* We have a comment! */
                getLine(false);
                return "\n";
            } else {
                isSym = true;
            }
        } else {
            if (!isStr) {
                if (c == ';') {
                    /* We have a comment! */
                    getLine(false);
                    break;
                } else if (c == '/' && data.back() == '/') {
                    /* We have a comment! */
                    getLine(false);

                    /* Is the comment all that was in the string? */
                    if(data.size() == 1)
                        return "\n";

                    /* Remove the other '/' character. */
                    data.pop_back();
                    break;
                } else if((std::isspace(c) || /* A space ends anything. */
                        /* Words break on non alpha-numeric characters other than underscores. */
                        (isWrd && !std::isalnum(c) && c != '_') ||
                        /* Numbers break on anything that isn't a digit or a period. */
                        (isNmb && !std::isdigit(c) && c != '.') ||
                        /* Symbols break on anything alpha-numeric. */
                        (isSym && std::isalnum(c)))) {
                    archive.ungetChar(*this, c);
                    break;
                }
            } else if (c == '"') {
                data += c;
                break;
            }
        }
        data += c;
    }

    return data;
}

integer BigEntry::getLineNumber() const {
    if (line < 0) {
        /* Line number is invalid. Go back and count lines in the file. */
        uint32_t pos = tell();
        seek(0);
        resetLineNumber();

        for (uint32_t i = 0; i < pos; ++i)
            getChar();
    }

    return line;
}

integer BigEntry::incrementLineNumber() const {
    if (line >= 0)
        return ++line;

    /* Delegate to getLineNumber to calculate the current line. */
    return getLineNumber();
}

integer BigEntry::decrementLineNumber() const {
    if (line >= 1)
        return --line;

    /* Delegate to getLineNumber to calculate the current line. */
    return getLineNumber();
}

}
