
# Set the compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Include directories
INCLUDES = -I./tests -I./tokenizer -I./parser -I./evaluator

# Source directories
TESTS_DIR = tests
TOKENIZER_DIR = tokenizer
PARSER_DIR = parser
EVAL_DIR = evaluator

# Output directories
BIN_DIR = bin
OBJ_DIR = .o

# Library directories and linking
LIBDIR = ./library/bin
LIBS = -lAsher -lc

# rpath setting (this will ensure the shared libraries are found)
RPATH = -Wl,-rpath=$(LIBDIR)

# Find all .cpp files in the tests directory
TEST_SOURCES = $(wildcard $(TESTS_DIR)/*.cpp)
TEST_BINS = $(patsubst $(TESTS_DIR)/%.cpp, $(BIN_DIR)/%, $(TEST_SOURCES))

# Main binary
MAIN_SRC = main.cpp
MAIN_BIN = $(BIN_DIR)/main

# Object files (for modularity and reuse)
MAIN_OBJ = $(OBJ_DIR)/main.o
TOKENIZER_OBJS = $(patsubst $(TOKENIZER_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(TOKENIZER_DIR)/*.cpp))
PARSER_OBJS = $(patsubst $(PARSER_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(PARSER_DIR)/*.cpp))
EVALUATOR_OBJS = $(patsubst $(EVAL_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(wildcard $(EVAL_DIR)/*.cpp))

# Default target: build all binaries
all: $(TEST_BINS) $(MAIN_BIN)

# Rule to build main binary
$(MAIN_BIN): $(MAIN_OBJ) $(TOKENIZER_OBJS) $(PARSER_OBJS) $(EVALUATOR_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ -L$(LIBDIR) $(LIBS) $(RPATH)

# Rule to build each test binary
$(BIN_DIR)/%: $(TESTS_DIR)/%.cpp $(TOKENIZER_OBJS) $(PARSER_OBJS) $(EVALUATOR_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(TOKENIZER_OBJS) $(PARSER_OBJS) $(EVALUATOR_OBJS) -o $@ -L$(LIBDIR) $(LIBS) $(RPATH)

# Rule to compile main.o
$(OBJ_DIR)/main.o: $(MAIN_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile object files for tokenizer
$(OBJ_DIR)/%.o: $(TOKENIZER_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile object files for parser
$(OBJ_DIR)/%.o: $(PARSER_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Rule to compile object files for evaluator
$(OBJ_DIR)/%.o: $(EVAL_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up the compiled files and binaries
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

.PHONY: all clean
