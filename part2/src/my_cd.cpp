#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <filesystem>
#include "acl.h"
#include "funs.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <target>" << endl;
        return 1;
    }

    string target = argv[1];
    filesystem::path targetPath = filesystem::absolute(target);

    if (!filesystem::exists(targetPath)) {
        cerr << "Error: " << targetPath.string() << " does not exist" << endl;
        return 1;
    }

    uid_t targetOwner = Funs::getFileOwnerUID(target);
    if (seteuid(targetOwner) < 0) {
        cerr << "Error: Could not change to owner of " << targetPath.string() << endl;
        return 1;
    }

    AccessController ac;
    bool aclLoaded = ac.read_from_file(target);
    if (aclLoaded) {
        if (!ac.has_permission(getuid(), 4)) {
            cerr << "Error: You do not have read permission for " << targetPath.string() << endl;
            seteuid(getuid());
            return 1;
        }
    } else {
        if (getuid() != targetOwner) {
            cerr << "Error: No ACL present and you are not the owner of " << targetPath.string() << endl;
            seteuid(getuid());
            return 1;
        }
    }

    if (seteuid(getuid()) < 0) {
        cerr << "Error: Could not revert effective UID" << endl;
        return 1;
    }

    cout << "You have valid read permission for " << targetPath.string() << endl;
    return 0;
}
