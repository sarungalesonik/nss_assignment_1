#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include "access_controller.h"
#include "funs.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: setacl <file_path> <user_name> <permissions>" << endl;
        return 1;
    }

    string filePath = argv[1];
    string userName = argv[2];
    string permissions = argv[3];

    struct stat fileStats;
    if (stat(filePath.c_str(), &fileStats) != 0) {
        cerr << "Error: Unable to find the specified file" << endl;
        return 1;
    }
    if (getuid() != fileStats.st_uid) {
        cerr << "Error: Only the file owner is permitted to modify the ACL" << endl;
        return 1;
    }

    AccessController aclController;
    if (!aclController.read_from_file(filePath)) {
        aclController.define_owner(fileStats.st_uid);
    }

    uid_t targetUserId = AccessController::user_to_id(userName);
    if (targetUserId == static_cast<uid_t>(-1)) {
        cerr << "Error: User not found" << endl;
        return 1;
    }

    int permissionFlags = AccessController::decode_permissions(permissions);
    aclController.grant_access(targetUserId, permissionFlags);

    if (!aclController.write_to_file(filePath)) {
        cerr << "Error: Failed to save updated ACL" << endl;
        return 1;
    }

    cout << "Access control list updated successfully for " << filePath << endl;
    return 0;
}
