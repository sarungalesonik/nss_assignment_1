#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <filesystem>

namespace Funs{
    uid_t getUIDFromUsername(const std::string& userName);
    uid_t getFileOwnerUID(const std::string& filePath);
    int convertPermissionString(const std::string& permStr);
    std::string getAbsolutePath(const std::string& filePath);
    std::string getUsernameFromUID(uid_t userID);
    std::string formatPermission(int permValue);
}

#endif 
