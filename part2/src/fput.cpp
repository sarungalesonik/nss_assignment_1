#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <filesystem>
#include <cstdlib>

#include "access_controller.h"
#include "funs.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <path> \"<text>\"" << endl;
        return 1;
    }

    string targetFilePath = argv[1];
    string textToWrite = argv[2];
    filesystem::path absoluteFilePath = filesystem::absolute(targetFilePath);

    if (!filesystem::exists(absoluteFilePath)) {
        cerr << "Invalid path: " << absoluteFilePath.string() << " does not exist" << endl;
        return 1;
    }

    uid_t fileOwnerUID = Funs::getFileOwnerUID(targetFilePath);
    if (seteuid(fileOwnerUID) < 0) {
        cerr << "Unable to switch to the owner of " << targetFilePath << endl;
        return 1;
    }

    AccessController aclController;
    if (!aclController.read_from_file(targetFilePath)) {
        if (getuid() != fileOwnerUID) {
            cerr << "Permission denied for: " << targetFilePath << endl;
            return 1;
        }
        aclController.define_owner(getuid());
        aclController.grant_access(getuid(), 7);
        if (!aclController.write_to_file(targetFilePath)) {
            cerr << "Failed to save ACL for: " << targetFilePath << endl;
            return 1;
        }
    } else {
        if (!aclController.has_permission(getuid(), 2)) {
            cerr << "Write permission denied for: " << targetFilePath << endl;
            return 1;
        }
    }

    ofstream fileStream(targetFilePath, ios::app);
    if (!fileStream) {
        cerr << "Unable to open " << targetFilePath << " for writing" << endl;
        return 1;
    }

    fileStream << textToWrite << endl;
    if (fileStream.fail() || fileStream.bad()) {
        cerr << "Failed to write to: " << targetFilePath << endl;
        return 1;
    }
    fileStream.close();

    if (seteuid(getuid()) < 0) {
        cerr << "Unable to revert to original user" << endl;
        return 1;
    }

    cout << "Successfully wrote to: " << targetFilePath << endl;
    return 0;
}
