CC = clang++
CFLAGS = -std=c++17 -I./src -I/usr/local/include
LIBS = -L/usr/local/lib -lboost_serialization -lboost_system

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

# Source files
GETACL_SRC = $(SRC_DIR)/getacl.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
SETACL_SRC = $(SRC_DIR)/setacl.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
FGET_SRC = $(SRC_DIR)/fget.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
FPUT_SRC = $(SRC_DIR)/fput.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
MY_CD_SRC = $(SRC_DIR)/my_cd.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
MY_LS_SRC = $(SRC_DIR)/my_ls.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp
CREATE_DIR_SRC = $(SRC_DIR)/create_dir.cpp $(SRC_DIR)/helper.cpp $(SRC_DIR)/acl.cpp

# Object files (stored in obj directory)
GETACL_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(GETACL_SRC))
SETACL_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SETACL_SRC))
FGET_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(FGET_SRC))
FPUT_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(FPUT_SRC))
MY_CD_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MY_CD_SRC))
MY_LS_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MY_LS_SRC))
CREATE_DIR_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CREATE_DIR_SRC))

# Output binary names
GETACL_BIN = $(BIN_DIR)/getacl
SETACL_BIN = $(BIN_DIR)/setacl
FGET_BIN = $(BIN_DIR)/fget
FPUT_BIN = $(BIN_DIR)/fput
MY_CD_BIN = $(BIN_DIR)/my_cd
MY_LS_BIN = $(BIN_DIR)/my_ls
CREATE_DIR_BIN = $(BIN_DIR)/create_dir
ACLSHELL_BIN = ACLShell

# Default target: Build all executables
all: $(GETACL_BIN) $(SETACL_BIN) $(FGET_BIN) $(FPUT_BIN) $(MY_CD_BIN) $(MY_LS_BIN) $(CREATE_DIR_BIN) $(ACLSHELL_BIN)

# Rules to create executables
$(GETACL_BIN): $(GETACL_OBJ)
	$(CC) $(CFLAGS) -o $(GETACL_BIN) $(GETACL_OBJ) $(LIBS)

$(SETACL_BIN): $(SETACL_OBJ)
	$(CC) $(CFLAGS) -o $(SETACL_BIN) $(SETACL_OBJ) $(LIBS)

$(FGET_BIN): $(FGET_OBJ)
	$(CC) $(CFLAGS) -o $(FGET_BIN) $(FGET_OBJ) $(LIBS)

$(FPUT_BIN): $(FPUT_OBJ)
	$(CC) $(CFLAGS) -o $(FPUT_BIN) $(FPUT_OBJ) $(LIBS)

$(MY_CD_BIN): $(MY_CD_OBJ)
	$(CC) $(CFLAGS) -o $(MY_CD_BIN) $(MY_CD_OBJ) $(LIBS)

$(MY_LS_BIN): $(MY_LS_OBJ)
	$(CC) $(CFLAGS) -o $(MY_LS_BIN) $(MY_LS_OBJ) $(LIBS)

$(CREATE_DIR_BIN): $(CREATE_DIR_OBJ)
	$(CC) $(CFLAGS) -o $(CREATE_DIR_BIN) $(CREATE_DIR_OBJ) $(LIBS)

$(ACLSHELL_BIN): $(SRC_DIR)/ACLShell.cpp
	c++ -o $(ACLSHELL_BIN) $(SRC_DIR)/ACLShell.cpp

# Rule to compile .cpp to .o (store objects in obj directory)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure obj directory exists
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all generated files
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* $(ACLSHELL_BIN)