#include "funs.h"
#include <filesystem>
#include <sys/stat.h>
#include <pwd.h>

namespace Funs{

    uid_t getUIDFromUsername(const std::string& userName) {
        if (passwd* pwEntry = getpwnam(userName.c_str())) {
            return pwEntry->pw_uid;
        }
        return static_cast<uid_t>(-1);
    }
    
    uid_t getFileOwnerUID(const std::string& filePath) {
        std::filesystem::path absPath = std::filesystem::absolute(filePath);
        struct stat fileStatus;
        if (stat(absPath.c_str(), &fileStatus) == 0) {
            return fileStatus.st_uid;
        }
        return static_cast<uid_t>(-1);
    }
    

    std::string getAbsolutePath(const std::string& filePath) {
        std::filesystem::path absPath = std::filesystem::absolute(filePath);
        return absPath.string();
    }

    std::string getUsernameFromUID(uid_t userID) {
        if (passwd* pwEntry = getpwuid(userID)) {
            return std::string(pwEntry->pw_name);
        }
        return {};
    }


    std::string formatPermission(int permValue) {
        std::string result;
        result.push_back((permValue & 4) ? 'r' : '-');
        result.push_back((permValue & 2) ? 'w' : '-');
        result.push_back((permValue & 1) ? 'x' : '-');
        return result;
    }

    int convertPermissionString(const std::string& permStr) {
        int permValue = 0;
        for (char ch : permStr) {
            switch(ch) {
                case 'r': permValue |= 4; break;
                case 'w': permValue |= 2; break;
                case 'x': permValue |= 1; break;
                default: break;
            }
        }
        return permValue;
    }

}
