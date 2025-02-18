#include <iostream>
#include <string>
#include <sys/stat.h>
#include <cstdlib>
#include "access_controller.h"
#include "funs.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: setacl <file> <user> <permissions>" << endl;
        cerr << "Example: setacl myfile.txt alice rwx" << endl;
        return 1;
    }

    string filePath = argv[1];
    string userName = argv[2];
    string rightsStr = argv[3];

    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0) {
        cerr << "Error: File not found" << endl;
        return 1;
    }
    if (getuid() != fileStat.st_uid) {
        cerr << "Error: Only file owner can modify ACL" << endl;
        return 1;
    }

    AccessController acl;
    if (!acl.read_from_file(filePath)) {
        acl.define_owner(fileStat.st_uid);
    }

    uid_t targetUid = AccessController::user_to_id(userName);
    if (targetUid == static_cast<uid_t>(-1)) {
        cerr << "Error: Invalid user" << endl;
        return 1;
    }

    int rights = AccessController::decode_permissions(rightsStr);
    acl.grant_access(targetUid, rights);

    if (!acl.write_to_file(filePath)) {
        cerr << "Error: Failed to save ACL" << endl;
        return 1;
    }

    cout << "ACL updated successfully" << endl;
    return 0;
}
