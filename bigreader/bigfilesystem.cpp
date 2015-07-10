#include "bigentry.hpp"
#include "bigarchive.hpp"
#include "bigfilesystem.hpp"
#include "log.hpp"
#include <algorithm>
#include <list>
#include <regex>

namespace OpenBFME {

namespace BigFilesystem {

namespace {
std::list<BigArchive> archives;
}

BigArchive *mount(const string& filename, bool append) {
    Log::info("Attempting to mount \"%s\" in %s mode", filename, append ? "append" : "prepend");

    if (append) {
        archives.emplace_front(filename);

        BigArchive &newArchive = archives.front();

        if (newArchive.readHeader())
            return &newArchive;

        archives.pop_front();
    } else {
        archives.emplace_back(filename);

        BigArchive &newArchive = archives.back();

        if (newArchive.readHeader())
            return &newArchive;

        archives.pop_back();
    }
    Log::error("Unable to mount \"%s\"!", filename);
    return nullptr;
}

bool unmount(const string& filename){
    Log::info("Unmounting \"%s\"", filename);

    for (auto i = archives.begin(); i != archives.end(); ++i) {
        if (i->getArchiveFilename() == filename) {
            archives.erase(i);
            return true;
        }
    }
    return false;
}

bool unmount(BigArchive* archive){
    Log::info("Unmounting \"%s\"", archive->getArchiveFilename());

    for (auto i = archives.begin(); i != archives.end(); ++i) {
        if (&*i == archive) {
            archives.erase(i);
            return true;
        }
    }
    return false;
}

const BigEntry* openFile(const string& filename, const string& relativeTo) {
    string fullPath = filename;
    /* Everything uses '/' as the folder delimiter. */
    std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

    if (!relativeTo.empty()) {
        /* Put the relativeTo path at the start of the string, minus anything after the last delimiter. */
        fullPath.insert(0, relativeTo.substr(0, relativeTo.find_last_of("\\/") + 1));
        std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

        /* If there are any instances of "../" in the path remove the previous folder name. */
        for (string::size_type p, folder; (p = fullPath.find("../")) != string::npos;) {
            if(p == 0)
                folder = 0;
            else
                folder = fullPath.find_last_of('/', p - 2);

            if(folder == string::npos)
                folder = 0;

            /* Erase from the start of the folder to 3 characters after the end of it (the "../"). */
            fullPath.erase(folder, p - folder + 3);
        }
    }

    Log::info("Attempting to open file \"%s\", expanded to \"%s\"", filename, fullPath);

    /* Go through the archives in order and open the first file found with the correct filename. */
    for (BigArchive& archive : archives) {
        const BigEntry* entry = archive.openFile(fullPath);

        /* Don't return immediately if the file couldn't be opened, try the other archives. */
        if (entry != nullptr)
            return entry;
    }
    return nullptr;
}

/* TODO - Make a seperate version of the function that takes just basic wildcards. */
std::set<string> findFiles(const string &regexStr) {
    std::set<string> output;
    std::basic_regex<character> regex(regexStr);

    for (BigArchive& archive : archives) {
        for (const BigEntry& entry : archive) {
            if (regex_match(entry.filename, regex)) {
                output.emplace(entry.filename);
            }
        }
    }

    return std::move(output);
}

}

}
