#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <filesystem>
#include "access_controller.h"
#include "funs.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <target_path>" << endl;
        return 1;
    }

    string targetPathString = argv[1];
    filesystem::path absoluteTargetPath = filesystem::absolute(targetPathString);

    if (!filesystem::exists(absoluteTargetPath)) {
        cerr << "Invalid path: " << absoluteTargetPath.string() << " does not exist" << endl;
        return 1;
    }

    uid_t fileOwnerUID = Funs::getFileOwnerUID(targetPathString);
    if (seteuid(fileOwnerUID) < 0) {
        cerr << "Unable to switch to the owner of: " << absoluteTargetPath.string() << endl;
        return 1;
    }

    AccessController aclController;
    bool aclLoaded = aclController.read_from_file(targetPathString);
    if (aclLoaded) {
        if (!aclController.has_permission(getuid(), 4)) {
            cerr << "Permission denied: You do not have read access to " << absoluteTargetPath.string() << endl;
            seteuid(getuid());
            return 1;
        }
    } else {
        if (getuid() != fileOwnerUID) {
            cerr << "No ACL found and you are not the owner of: " << absoluteTargetPath.string() << endl;
            seteuid(getuid());
            return 1;
        }
    }

    if (seteuid(getuid()) < 0) {
        cerr << "Failed to revert to the original user" << endl;
        return 1;
    }

    cout << "You have valid read permission for: " << absoluteTargetPath.string() << endl;
    return 0;
}
