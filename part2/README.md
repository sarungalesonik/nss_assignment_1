# nss_assignment_1# Access Control Lists

## Description of System
- `fake_root` operates with UID 0, simulating root privileges.
- Each file/directory has its own ACL stored in Extended Attributes:

- The `Owner` and `User` fields store user IDs (UIDs).
- If an ACL does not exist for a file/directory during a write operation (`setacl`, `fput`, `create_dir`), one is created with the owner assigned and stored.

## Assumptions
- `my_cd` only goes further, not back.

## Usage

### Compilation
```
make
```

### Set Permissions
```
chmod u+s bin/*
```

### Cleaning Up
```
make clean
```

## Commands

### Setting ACL for a File
```
setacl <path> <user> <permissions>
```

### Retrieving ACL for a File
```
getacl <path>
```

### Changing Directory
```
my_cd <path>
```

### Creating a Directory
```
create_dir <path>
```

### Writing to a File
```
fput <path> "<contents>"
```

### Reading a File
```
fget <path>
```

### Listing Directory Contents
```
my_ls <path>
```

## Security Measures
1. **Directory Traversal & Unauthorized Creation Restrictions**  
   - `create_dir` ensures users cannot create directories unless they have write permissions in the parent directory. Additionally, it enforces `0755` permissions, preventing unauthorized modifications by non-owners.  

2. **Input Validation & Error Handling**  
   - The system ensures that:  
     - Invalid or non-existent paths return errors rather than failing silently.  
     - Attempting to modify or read a file/directory without the necessary permissions results in an appropriate error message.  
3. **Enforced Permission Isolation**  
   - Users must have appropriate ACL permissions to execute actions. For example:  
     - `my_ls` requires read (`r`) permission to list directory contents.  
     - `fput` requires write (`w`) permission to modify a file.  

4. **Unauthorized Access Prevention**  
   - If an ACL does not exist for a file or directory, one is created with the file owner as the ACL owner. Any other user must have explicit permissions set via `setacl` to perform operations.  