CXX = c++
CFLAGS = -c
LINKER = -o
CXXSOURCE = cmdline.cpp main.cpp
HEADERS = cmdline.hpp

all: msdscript

#this will compile the changed files
msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CFLAGS) $(CXXSOURCE)
	$(CXX) cmdline.o main.o $(LINKER) msdscript

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm msdscript