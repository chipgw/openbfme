#include "bigreader.h"
#include "log.h"
#include <cstdio>

namespace OpenBFME {

BigArchive *BigFilesystem::mount(const std::string &filename, bool append){
    Log::info("Attempting to mount \"%s\" in %s mode", filename.c_str(), append ? "append" : "prepend");

    if(append){
        archives.emplace_front(filename);

        BigArchive &newArchive = archives.front();

        if(newArchive.readHeader()){
            return &newArchive;
        }

        archives.pop_front();
    }else{
        archives.emplace_back(filename);

        BigArchive &newArchive = archives.back();

        if(newArchive.readHeader()){
            return &newArchive;
        }

        archives.pop_back();
    }
    Log::error("Unable to mount \"%s\"!", filename.c_str());
    return nullptr;
}

bool BigFilesystem::unmount(const std::string &filename){
    Log::info("Unmounting \"%s\"", filename.c_str());

    for(auto i = archives.begin(); i != archives.end(); ++i){
        if(i->archiveFilename == filename){
            archives.erase(i);
            return true;
        }
    }
    return false;
}

bool BigFilesystem::unmount(BigArchive* archive){
    Log::info("Unmounting \"%s\"", archive->archiveFilename.c_str());

    for(auto i = archives.begin(); i != archives.end(); ++i){
        if(&*i == archive){
            archives.erase(i);
            return true;
        }
    }
    return false;
}

BigEntry* BigFilesystem::openFile(const std::string &filename){
    Log::info("attempting to open file \"%s\"", filename.c_str());
    for(BigArchive &archive : archives){
        BigEntry* entry = archive.openFile(filename);
        if(entry != nullptr){
            return entry;
        }
    }
    return nullptr;
}

}
