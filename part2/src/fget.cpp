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

    string filePath = argv[1];
    filesystem::path absPath = filesystem::absolute(filePath);

    if (!filesystem::exists(absPath) ||
       (!filesystem::is_regular_file(absPath) && !filesystem::is_directory(absPath))) {
        cerr << "Error: " << absPath.string() << " is not a valid file or directory" << endl;
        return 1;
    }

    if (seteuid(Funs::getFileOwnerUID(filePath)) < 0) {
        cerr << "Error: Could not change to owner of " << filePath << endl;
        return 1;
    }

    AccessController ac;
    if (!ac.read_from_file(filePath)) {
        cerr << "Error: ACL not found for " << filePath << endl;
        return 1;
    }

    if (!ac.has_permission(getuid(), 4)) {
        cerr << "Error: You do not have read permissions for " << filePath << endl;
        return 1;
    }

    ifstream inFile(filePath);
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();

    if (seteuid(getuid()) < 0) {
        cerr << "Error: Could not change back to original user" << endl;
        return 1;
    }
    return 0;
}
