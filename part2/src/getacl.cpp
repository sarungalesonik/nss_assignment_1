#include <iostream>
#include <string>
#include "acl.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: getacl <file>" << endl;
        return 1;
    }

    string filePath = argv[1];

    AccessController ac;
    if (!ac.read_from_file(filePath)) {
        cerr << "Error: No ACL found" << endl;
        return 1;
    }

    ac.display();
    return 0;
}
