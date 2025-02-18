#include <iostream>
#include <string>
#include "access_controller.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: getacl <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];

    AccessController aclController;
    if (!aclController.read_from_file(filePath)) {
        cerr << "Error: ACL not found for file " << filePath << endl;
        return 1;
    }

    aclController.display();
    return 0;
}
