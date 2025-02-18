#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

void exit_with_error(const string &msg) {
    perror(msg.c_str());
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <command> [args...]" << endl;
        return EXIT_FAILURE;
    }

    string cmd = argv[1];
    char **args = &argv[1];
    char path_buf[1024];
    struct stat sb;

    if (cmd.find('/') == string::npos) {
        const char *path_env = getenv("PATH");
        if (!path_env) exit_with_error("PATH not set");

        char *path_dup = strdup(path_env);
        if (!path_dup) exit_with_error("strdup failed");

        bool found = false;
        for (char *token = strtok(path_dup, ":"); token; token = strtok(nullptr, ":")) {
            snprintf(path_buf, sizeof(path_buf), "%s/%s", token, cmd.c_str());
            if (access(path_buf, F_OK) == 0) {
                found = true;
                break;
            }
        }
        free(path_dup);
        if (!found) {
            cerr << "Command '" << cmd << "' not found" << endl;
            return EXIT_FAILURE;
        }
    } else {
        snprintf(path_buf, sizeof(path_buf), "%s", cmd.c_str());
        if (access(path_buf, F_OK) != 0) {
            cerr << "Command '" << cmd << "' not found" << endl;
            return EXIT_FAILURE;
        }
    }

    if (stat(path_buf, &sb) != 0) exit_with_error("stat failed");

    if (!S_ISREG(sb.st_mode)) {
        cerr << "'" << path_buf << "' is not a regular file" << endl;
        return EXIT_FAILURE;
    }

    uid_t orig_uid = getuid();
    uid_t owner_uid = sb.st_uid;

    // Changing the user id to the owner of the executable
    if (seteuid(owner_uid) != 0) exit_with_error("seteuid to owner failed");


    // Executing the command
    execv(path_buf, args);

    // Changing the user id back to the original user if execv fails
    if (setuid(orig_uid) != 0) exit_with_error("failed to revert effective UID");

    exit_with_error("execv failed");
}
