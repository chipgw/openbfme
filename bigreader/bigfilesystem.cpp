#include "bigreader.hpp"
#include "log.hpp"
#include <algorithm>

namespace OpenBFME {

BigArchive *BigFilesystem::mount(const string &filename, bool append){
    Log::info("Attempting to mount \"%s\" in %s mode", filename.c_str(), append ? "append" : "prepend");

    if(append){
        archives.emplace_front(filename, *this);

        BigArchive &newArchive = archives.front();

        if(newArchive.readHeader()){
            return &newArchive;
        }

        archives.pop_front();
    }else{
        archives.emplace_back(filename, *this);

        BigArchive &newArchive = archives.back();

        if(newArchive.readHeader()){
            return &newArchive;
        }

        archives.pop_back();
    }
    Log::error("Unable to mount \"%s\"!", filename.c_str());
    return nullptr;
}

bool BigFilesystem::unmount(const string &filename){
    Log::info("Unmounting \"%s\"", filename.c_str());

    for(auto i = archives.begin(); i != archives.end(); ++i){
        if(i->getArchiveFilename() == filename){
            archives.erase(i);
            return true;
        }
    }
    return false;
}

bool BigFilesystem::unmount(BigArchive* archive){
    Log::info("Unmounting \"%s\"", archive->getArchiveFilename().c_str());

    for(auto i = archives.begin(); i != archives.end(); ++i){
        if(&*i == archive){
            archives.erase(i);
            return true;
        }
    }
    return false;
}

const BigEntry* BigFilesystem::openFile(const string &filename, const string &relativeTo){
    string fullPath = filename;
    std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

    if(!relativeTo.empty() && fullPath.find("../") != string::npos){
        fullPath.insert(0, relativeTo.substr(0, relativeTo.find_last_of("\\/")) + '/');
        std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

        for(string::size_type p, folder; (p = fullPath.find("../")) != string::npos;){
            if(p == 0){
                folder = 0;
            }else{
                folder = fullPath.find_last_of('/', p - 2);
            }

            if(folder == string::npos){
                folder = 0;
            }

            fullPath.erase(folder, p - folder + 3);
        }
    }

    Log::info("attempting to open file \"%s\", expanded to \"%s\"", filename.c_str(), fullPath.c_str());
    for(BigArchive &archive : archives){
        const BigEntry* entry = archive.openFile(fullPath);
        if(entry != nullptr){
            return entry;
        }
    }
    return nullptr;
}

}
