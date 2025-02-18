CXX = c++
CLANG = clang++
CXXFLAGS = -I./src -I/usr/local/include
STD = -std=c++17
BOOSTLIBS = -L/usr/local/lib -lboost_serialization -lboost_system

# Directories
OBJDIR = obj
BINDIR = bin

# Default target: build everything in sequence
all: directories ACLShell bin/fget bin/create_dir bin/fput bin/my_cd bin/my_ls bin/setacl bin/getacl

# Create directories if they do not exist
directories:
        @mkdir -p $(OBJDIR) $(BINDIR)

# 1. Build ACLShell
ACLShell: src/ACLShell.cpp
        $(CXX) -o ACLShell src/ACLShell.cpp

# 2. Compile object files in order
$(OBJDIR)/funs.o: src/funs.cpp
        $(CXX) $(CXXFLAGS) -c src/funs.cpp -o $(OBJDIR)/funs.o

$(OBJDIR)/access_controller.o: src/access_controller.cpp
        $(CXX) $(CXXFLAGS) -c src/access_controller.cpp -o $(OBJDIR)/access_controller.o

$(OBJDIR)/fget.o: src/fget.cpp
        $(CXX) $(CXXFLAGS) -c src/fget.cpp -o $(OBJDIR)/fget.o

$(OBJDIR)/create_dir.o: src/create_dir.cpp
        $(CXX) $(CXXFLAGS) -c src/create_dir.cpp -o $(OBJDIR)/create_dir.o

$(OBJDIR)/fput.o: src/fput.cpp
        $(CXX) $(CXXFLAGS) -c src/fput.cpp -o $(OBJDIR)/fput.o

$(OBJDIR)/my_cd.o: src/my_cd.cpp
        $(CXX) $(CXXFLAGS) -c src/my_cd.cpp -o $(OBJDIR)/my_cd.o

$(OBJDIR)/my_ls.o: src/my_ls.cpp
        $(CXX) $(CXXFLAGS) -c src/my_ls.cpp -o $(OBJDIR)/my_ls.o

$(OBJDIR)/setacl.o: src/setacl.cpp
        $(CXX) $(CXXFLAGS) -c src/setacl.cpp -o $(OBJDIR)/setacl.o

$(OBJDIR)/getacl.o: src/getacl.cpp
        $(CXX) $(CXXFLAGS) -c src/getacl.cpp -o $(OBJDIR)/getacl.o

# 3. Link binaries with clang++ using C++17
bin/fget: $(OBJDIR)/fget.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/fget.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/create_dir: $(OBJDIR)/create_dir.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/create_dir.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/fput: $(OBJDIR)/fput.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/fput.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/my_cd: $(OBJDIR)/my_cd.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/my_cd.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/my_ls: $(OBJDIR)/my_ls.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/my_ls.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/setacl: $(OBJDIR)/setacl.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/setacl.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

bin/getacl: $(OBJDIR)/getacl.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o
        $(CLANG) $(STD) $(CXXFLAGS) -o $@ $(OBJDIR)/getacl.o $(OBJDIR)/funs.o $(OBJDIR)/access_controller.o $(BOOSTLIBS)

# Clean target to remove generated files
clean:
        rm -f ACLShell
        rm -rf $(OBJDIR) $(BINDIR)