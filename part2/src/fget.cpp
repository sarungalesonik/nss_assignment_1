#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <cstdlib>
#include "funs.h"
#include "access_controller.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <path>" << endl;
        return 1;
    }

    string targetPath = argv[1];
    filesystem::path absolutePath = filesystem::absolute(targetPath);

    if (!filesystem::exists(absolutePath) ||
       (!filesystem::is_regular_file(absolutePath) && !filesystem::is_directory(absolutePath))) {
        cerr << "Invalid path: " << absolutePath.string() << " is neither a file nor a directory" << endl;
        return 1;
    }

    uid_t fileOwnerUID = Funs::getFileOwnerUID(targetPath);
    if (seteuid(fileOwnerUID) < 0) {
        cerr << "Unable to switch to the owner of: " << targetPath << endl;
        return 1;
    }

    AccessController aclController;
    if (!aclController.read_from_file(targetPath)) {
        cerr << "ACL not found for: " << targetPath << endl;
        return 1;
    }

    if (!aclController.has_permission(getuid(), 4)) {
        cerr << "You do not have read permission for: " << targetPath << endl;
        return 1;
    }

    ifstream fileStream(targetPath);
    string fileLine;
    while (getline(fileStream, fileLine)) {
        cout << fileLine << endl;
    }
    fileStream.close();

    if (seteuid(getuid()) < 0) {
        cerr << "Unable to revert to original user" << endl;
        return 1;
    }

    return 0;
}
