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

    string filePath = argv[1];
    string text = argv[2];
    filesystem::path absPath = filesystem::absolute(filePath);

    if (!filesystem::exists(absPath)) {
        cerr << "Error: " << absPath.string() << " does not exist" << endl;
        return 1;
    }

    uid_t fileOwner = Funs::getFileOwnerUID(filePath);
    if (seteuid(fileOwner) < 0) {
        cerr << "Error: Could not switch to owner of " << filePath << endl;
        return 1;
    }

    AccessController ac;
    if (!ac.read_from_file(filePath)) {
        if (getuid() != fileOwner) {
            cerr << "Error: Permission denied for " << filePath << endl;
            return 1;
        }
        ac.define_owner(getuid());
        ac.grant_access(getuid(), 7);
        if (!ac.write_to_file(filePath)) {
            cerr << "Error: Could not save ACL for " << filePath << endl;
            return 1;
        }
    } else {
        if (!ac.has_permission(getuid(), 2)) {
            cerr << "Error: Write permission denied for " << filePath << endl;
            return 1;
        }
    }

    ofstream outFile(filePath, ios::app);
    if (!outFile) {
        cerr << "Error: Unable to open " << filePath << " for writing" << endl;
        return 1;
    }

    outFile << text << endl;
    if (outFile.fail() || outFile.bad()) {
        cerr << "Error: Failed to write to " << filePath << endl;
        return 1;
    }
    outFile.close();

    if (seteuid(getuid()) < 0) {
        cerr << "Error: Could not revert to original user" << endl;
        return 1;
    }

    cout << "Successfully wrote to " << filePath << endl;
    return 0;
}
