# nss_assignment_1# My Sudo

## Description of System
- **Privilege Handling** 
   - The program checks the ownership of the target executable using stat(). If the executable is owned by root, it runs with root privileges; otherwise, it switches to the non-root user using seteuid(), ensuring proper privilege separation.

- **Command Execution Flow**
   - The program searches for the command in the PATH if no absolute path is provided, verifies its existence and type, and then executes it using execv(). If execution fails, it reverts to the original user ID to prevent privilege escalation risks.

### Assumptions
* The program (my_sudo) must be owned by root and have the setuid bit enabled.
* The target executable must exist, be a regular file, allowing it to be properly located and run by execv()

### Usage

* `make` to compile
* `make clean` to remove executables

### Testing
* Running `my_sudo` :-
```bash
Usage: ./my_sudo <command> [args...]
```

### Attacks/Bugs/Errors Defended against
* By manually searching through PATH and constructing the full path, the program avoids blindly trusting user-modified PATH values that could lead to running a malicious binary.
* If the command is missing, it prints "Command not found" instead of executing a wrong or malicious program.
* The program resets privileges using setuid(orig_uid) to prevent unauthorized privilege retention.
* The program only switches to the owner of the target executable, ensuring users cannot manually specify another UID to escalate privileges improperly