# Define variables
CXX = c++
CFLAGS = --std=c++11
LINKER = -o
CXXSOURCE = cmdline.cpp Expr.cpp main.cpp Tests.cpp
HEADERS = cmdline.hpp Expr.hpp catch.h
OBJFILES = cmdline.o main.o Expr.o Tests.o
LIBRARY = ../Library_MSDScript/libmsdscript.a

all: msdscript

# Generate object files for source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -c -o $@ $<

# Link the object files to create the executable
msdscript: $(OBJFILES)
	$(CXX) $(CFLAGS) $(LINKER) $@ $^
	
msdscript_from_Library: $(HEADERS) main.cpp
	c++ --std=c++11 -o msdscript_from_lib main.cpp -L../Library_MSDScript -lmsdscript

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
	
#msdscript_from_Library: $(LIBRARY)
#	$(CXX) $(CFLAGS) -o $@ main.cpp -L ../Library_MSDScript -lmsdscript
#
#$(LIBRARY): $(OBJECTS)
#	ar -ruv $@ $(OBJECTS)


