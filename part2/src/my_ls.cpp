#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "acl.h"
#include "funs.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    string currentDir;
    if (argc == 2) {
        string dir = argv[1];
        fs::path absPath = fs::absolute(dir);
        if (!fs::exists(absPath) || !fs::is_directory(absPath)) {
            cerr << "Error: " << absPath.string() << " is not a valid directory" << endl;
            return 1;
        }
        currentDir = absPath.string();
    } else {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            currentDir = fs::absolute(cwd).string();
        } else {
            cerr << "Error: Unable to retrieve current directory" << endl;
            return 1;
        }
    }

    AccessController ac;
    if (!ac.read_from_file(currentDir)) {
        cerr << "Error: Could not load ACL for directory: " << currentDir << endl;
        return 1;
    }

    if (!ac.has_permission(getuid(), 4)) {
        cerr << "Error: You do not have read permission for directory: " << currentDir << endl;
        return 1;
    }

    try {
        for (const auto& entry : fs::directory_iterator(currentDir)) {
            cout << entry.path().filename().string() << endl;
        }
    } catch (const fs::filesystem_error& err) {
        cerr << "Error: Unable to list directory contents: " << err.what() << endl;
        return 1;
    }

    return 0;
}
