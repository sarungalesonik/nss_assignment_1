#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <sstream>
#include <filesystem>
#include <cstdlib>

using namespace std;

const vector<string> reservedCmds = {
    "fput", "my_ls", "fget", "my_cd", "create_dir", "getacl", "setacl"
};

string currentDirectory;
string homeDirectory;

bool isReservedCmd(const string &cmd) {
    for (const auto &rc : reservedCmds) {
        if (cmd == rc) return true;
    }
    return false;
}

void runCommand(const string &commandLine) {
    if (chdir(currentDirectory.c_str()) != 0)
        cerr << "Warning: Cannot switch to " << currentDirectory << endl;

    vector<string> tokens;
    istringstream iss(commandLine);
    string token;
    while (iss >> token)
        tokens.push_back(token);
    if (tokens.empty()) return;

    vector<char*> args;
    for (const auto &arg : tokens)
        args.push_back(strdup(arg.c_str()));
    args.push_back(nullptr);

    bool isCdCmd = (tokens[0] == "my_cd");
    if (isReservedCmd(tokens[0])) {
        string binPath = homeDirectory + "/bin/" + tokens[0];
        free(args[0]);
        args[0] = strdup(binPath.c_str());
    }

    pid_t pid = fork();
    if (pid == 0) {
        if (isReservedCmd(tokens[0]))
            execvp(args[0], args.data());
        else
            execl("/bin/sh", "sh", "-c", commandLine.c_str(), nullptr);
        cerr << "Error: Command execution failed." << endl;
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (isCdCmd && WIFEXITED(status) && WEXITSTATUS(status) == 0 && tokens.size() >= 2) {
            currentDirectory = std::filesystem::absolute(tokens[1]).string();
            cout << "Updated shell directory: " << currentDirectory << endl;
        }
    } else {
        cerr << "Error: Fork unsuccessful." << endl;
    }

    for (auto argPtr : args)
        free(argPtr);
}

int main() {
    char *cwd = getcwd(nullptr, 0);
    if (cwd) {
        currentDirectory = string(cwd);
        homeDirectory = currentDirectory;
        free(cwd);
    } else {
        cerr << "Error: Unable to determine current directory." << endl;
        return 1;
    }
    string input;
    while (true) {
        cout << "ACLShell> ";
        getline(cin, input);
        if (input == "exit") break;
        runCommand(input);
    }
    return 0;
}
