# Define variables
CXX = c++
CFLAGS = --std=c++11 
LINKER = -o
CXXSOURCE = cmdline.cpp Expr.cpp main.cpp Tests.cpp parse.cpp Val.cpp
HEADERS = cmdline.hpp Expr.hpp catch.h parse.hpp Val.hpp
OBJFILES = cmdline.o main.o Expr.o Tests.o parse.o Val.o

all: msdscript

# Generate object files for source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link the object files to create the executable
msdscript: $(OBJFILES)
	$(CXX) $(CFLAGS) $(LINKER) $@ $^

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm -f msdscript
	
# 'make run' will run the executable
run: msdscript
	./msdscript

# 'make run test' will run the executable w/ the test tag
run test: msdscript
	./msdscript --test

doc: msdscript
	cd documentation && doxygen
	
# 'make test_msdscript' will create an executable for random test generation
test_msdscript: test_msdscript.o exec.o expr.o
	$(CXX) expr.o exec.o test_msdscript.o -o test_msdscript

test_msdscript.o: test_msdscript.cpp
	$(CXX) $(CFLAGS) -c test_msdscript.cpp -o test_msdscript.o

exec.o: exec.cpp exec.hpp
	$(CXX) $(CFLAGS) -c exec.cpp -o exec.o
