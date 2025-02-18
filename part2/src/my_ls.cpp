#include <iostream>
#include <filesystem>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "access_controller.h"
#include "funs.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    string targetDir;
    if (argc == 2) {
        string directoryPath = argv[1];
        fs::path absolutePath = fs::absolute(directoryPath);
        if (!fs::exists(absolutePath) || !fs::is_directory(absolutePath)) {
            cerr << "Invalid directory: " << absolutePath.string() << " is not a valid directory" << endl;
            return 1;
        }
        targetDir = absolutePath.string();
    } else {
        char currentWorkingDir[1024];
        if (getcwd(currentWorkingDir, sizeof(currentWorkingDir)) != nullptr) {
            targetDir = fs::absolute(currentWorkingDir).string();
        } else {
            cerr << "Unable to retrieve current directory" << endl;
            return 1;
        }
    }

    AccessController aclController;
    if (!aclController.read_from_file(targetDir)) {
        cerr << "Failed to load ACL for directory: " << targetDir << endl;
        return 1;
    }

    if (!aclController.has_permission(getuid(), 4)) {
        cerr << "Permission denied: You do not have read access to directory: " << targetDir << endl;
        return 1;
    }

    try {
        for (const auto& entry : fs::directory_iterator(targetDir)) {
            cout << entry.path().filename().string() << endl;
        }
    } catch (const fs::filesystem_error& error) {
        cerr << "Error: Unable to list directory contents: " << error.what() << endl;
        return 1;
    }

    return 0;
}
