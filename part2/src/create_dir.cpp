#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include "access_controller.h"
#include "funs.h"

using namespace std;
namespace fs = std::filesystem;

bool createDirAndSaveACL(const string &dirName) {
    if (mkdir(dirName.c_str(), 0755) == -1) {
        cerr << "Failed to create directory: " << strerror(errno) << endl;
        return false;
    }

    AccessController aclController;
    aclController.define_owner(getuid());
    aclController.grant_access(getuid(), 7);
    if (!aclController.write_to_file(dirName)) {
        cerr << "Failed to save ACL for directory: " << dirName << endl;
        return false;
    }

    cout << "Directory successfully created: " << dirName << endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <directory_name>" << endl;
        return 1;
    }
    
    string newDirectory = argv[1];

    char* currentDirPtr = getcwd(nullptr, 0);
    if (!currentDirPtr) {
        cerr << "Unable to get current directory: " << strerror(errno) << endl;
        return 1;
    }
    string currentDirectory(currentDirPtr);
    free(currentDirPtr);

    AccessController currentACL;
    bool aclLoaded = currentACL.read_from_file(currentDirectory);

    if (!aclLoaded) {
        uid_t currentOwner = Funs::getFileOwnerUID(currentDirectory);
        if (getuid() != currentOwner) {
            cerr << "You are not the owner of the directory." << endl;
            return 1;
        }
        currentACL.define_owner(getuid());
        currentACL.grant_access(getuid(), 7);
        if (!currentACL.write_to_file(currentDirectory)) {
            cerr << "Failed to save default ACL for current directory." << endl;
            return 1;
        }
    } else {
        if (!currentACL.has_permission(getuid(), 2)) {
            cerr << "You do not have the required write permission." << endl;
            return 1;
        }
        if (seteuid(currentACL.current_owner()) < 0) {
            cerr << "Failed to change effective UID: " << strerror(errno) << endl;
            return 1;
        }
    }
    
    if (!createDirAndSaveACL(newDirectory)) {
        cerr << "Failed to create directory " << newDirectory << endl;
        return 1;
    }
    
    if (seteuid(getuid()) < 0) {
        cerr << "Failed to revert EUID: " << strerror(errno) << endl;
        return 1;
    }
    
    return 0;
}
