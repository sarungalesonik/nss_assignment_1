#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include "acl.h"
#include "funs.h"

using namespace std;
namespace fs = std::filesystem;

bool createDirectoryAndSaveACL(const string &dirName) {
    if (mkdir(dirName.c_str(), 0755) == -1) {
        cerr << "Error: " << strerror(errno) << endl;
        return false;
    }

    AccessController aclCtrl;
    aclCtrl.define_owner(getuid());
    aclCtrl.grant_access(getuid(), 7);
    if (!aclCtrl.write_to_file(dirName)) {
        cerr << "Error: Could not save ACL for " << dirName << endl;
        return false;
    }

    cout << "Directory created successfully: " << dirName << endl;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <directory_name>" << endl;
        return 1;
    }
    
    string newDir = argv[1];

    char* cwdPtr = getcwd(nullptr, 0);
    if (!cwdPtr) {
        cerr << "Error: Unable to get current directory: " << strerror(errno) << endl;
        return 1;
    }
    string currentDir(cwdPtr);
    free(cwdPtr);

    AccessController currentACL;
    bool aclLoaded = currentACL.read_from_file(currentDir);

    if (!aclLoaded) {
        uid_t currentOwner = Funs::getFileOwnerUID(currentDir);
        if (getuid() != currentOwner) {
            cerr << "Error: You are not the owner of the current directory." << endl;
            return 1;
        }
        currentACL.define_owner(getuid());
        currentACL.grant_access(getuid(), 7);
        if (!currentACL.write_to_file(currentDir)) {
            cerr << "Error: Failed to save default ACL for the current directory." << endl;
            return 1;
        }
    } else {
        if (!currentACL.has_permission(getuid(), 2)) {
            cerr << "Error: You do not have write permission in the current directory." << endl;
            return 1;
        }
        if (seteuid(currentACL.current_owner()) < 0) {
            cerr << "Error: Could not change effective UID: " << strerror(errno) << endl;
            return 1;
        }
    }
    
    if (!createDirectoryAndSaveACL(newDir)) {
        cerr << "Error: Failed to create directory " << newDir << endl;
        return 1;
    }
    
    if (seteuid(getuid()) < 0) {
        cerr << "Error: Could not revert effective UID: " << strerror(errno) << endl;
        return 1;
    }
    
    return 0;
}
