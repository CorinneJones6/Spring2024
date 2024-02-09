# Define variables
CXX = c++
CFLAGS = --std=c++11
LINKER = -o
CXXSOURCE = main.cpp shelpers.cpp
HEADERS = shelpers.hpp
OBJFILES = main.o shelpers.o

all: program

# Generate object files for source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link the object files to create the executable
program: $(OBJFILES)
	$(CXX) $(CFLAGS) $(LINKER) $@ $^

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm -f msdscript
