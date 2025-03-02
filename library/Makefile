# Directories
SRC_DIR := implement
INC_DIR := include
BUILD_DIR := build
LIBRARY_DIR := bin

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Library output
LIB_NAME := libAsher.so
LIB_OUTPUT := $(LIBRARY_DIR)/$(LIB_NAME)

# Compiler and flags
CC := clang
CFLAGS := -fPIC -I$(INC_DIR) -O3 -w -Wall -Werror -g
LDFLAGS := -shared

# Default target
all: $(LIB_OUTPUT)

# Create the shared library
$(LIB_OUTPUT): $(OBJ_FILES)
	@mkdir -p $(LIBRARY_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile .c files to .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(LIBRARY_DIR)

.PHONY: all clean
