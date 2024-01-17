CXX = c++
CFLAGS = -c
LINKER = -o
CXXSOURCE = cmdline.cpp main.cpp Add.cpp Mult.cpp Expr.cpp Num.cpp VarExpr.cpp Tests.cpp
HEADERS = cmdline.hpp Add.hpp Mult.hpp Expr.hpp Num.hpp VarExpr.hpp Tests.hpp
OBJFILES = cmdline.o main.o Add.o Mult.o Expr.o Num.o VarExpr.o Tests.o

all: msdscript

#this will compile the changed files
msdscript: $(CXXSOURCE) $(HEADERS)
	$(CXX) $(CFLAGS) $(CXXSOURCE)
	$(CXX) $(OBJFILES) $(LINKER) msdscript

# Defines a target for cleaning up the project
.PHONY: clean

# 'make clean' will remove the executable and the .o files
clean:
	rm -rf *.o
	rm msdscript
