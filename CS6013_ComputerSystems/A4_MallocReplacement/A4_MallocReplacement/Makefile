# Define variables
CXX = c++
CFLAGS = --std=c++11
LINKER = -o
CXXSOURCE = HashTable.cpp Tests.cpp
HEADERS = HashTable.hpp
OBJFILES = HashTable.o Tests.o

all: executable

# Generate object files for source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link the object files to create the executable
executable: $(OBJFILES)
	$(CXX) $(CFLAGS) $(LINKER) $@ $^

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm -f executable
	
# 'make run' will run the executable
run: executable
	executable
