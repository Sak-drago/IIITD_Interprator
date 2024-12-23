# Set the compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra

# Include directories
INCLUDES = -I./tests -I./tokenizer

# Source files
SRC = tests/tokenizerTests.cpp tokenizer/tokenizer.cpp

# Output binary
OUT = bin/tokenizerTests

# Library directories and linking
LIBDIR = ./library/bin
LIBS = -lAsher -lc

# rpath setting (this will ensure the shared libraries are found)
RPATH = -Wl,-rpath=$(LIBDIR)

# The target to build the tests
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(OUT) -L$(LIBDIR) $(LIBS) $(RPATH)

# Clean up the compiled files
clean:
	rm -f $(OUT)

.PHONY: all clean run
