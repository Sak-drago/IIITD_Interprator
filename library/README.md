
# Forge Utilities Library

A simple C library for logging, assertions, and file operations.

## Table of Contents

1. [Introduction](#introduction)
2. [Log System](#log-system)
   - [Log Macros](#log-macros)
   - [Examples](#examples)
3. [Assertions](#assertions)
   - [Assert Macros](#assert-macros)
   - [Examples](#examples)
4. [File System Operations](#filesystem-operations)
   - [File Operations](#file-operations)
   - [Examples](#examples)
5. [Building and Linking](#building-and-linking)
6. [License](#license)

---

## Introduction

The Forge Utilities Library provides a set of tools for logging, assertions, and file operations, useful for debugging, error handling, and managing file I/O in C projects.

---

## Log System

The log system provides macros for printing log messages with various severity levels: Fatal, Error, Warning, Info, Debug, and Trace.

### Log Macros

| Macro                | Description                                      |
|----------------------|--------------------------------------------------|
| `FORGE_LOG_FATAL`    | Logs a fatal error and terminates execution.     |
| `FORGE_LOG_ERROR`    | Logs a critical error.                           |
| `FORGE_LOG_WARNING`  | Logs a warning if enabled.                       |
| `FORGE_LOG_INFO`     | Logs informational messages if enabled.          |
| `FORGE_LOG_DEBUG`    | Logs debug messages if enabled.                  |
| `FORGE_LOG_TRACE`    | Logs trace messages for detailed step tracking.  |

### Examples

```c
#include "logger.h"

void exampleLogging() 
{
    FORGE_LOG_FATAL("This is a fatal error!");
    FORGE_LOG_ERROR("An error occurred: %d", -1);
    FORGE_LOG_WARNING("Warning: Something might be wrong.");
    FORGE_LOG_INFO("Info: System is running as expected.");
    FORGE_LOG_DEBUG("Debug: Variable value is %d", 42);
    FORGE_LOG_TRACE("Trace: Entering function %s", __func__);
}
```

---

## Assertions

Assertions help enforce conditions and identify bugs by halting the program when an assertion fails.

### Assert Macros

| Macro                     | Description                                               |
|---------------------------|-----------------------------------------------------------|
| `FORGE_ASSERT`            | Checks an expression and halts the program if it fails.   |
| `FORGE_ASSERT_MESSAGE`    | Similar to `FORGE_ASSERT` but allows for custom messages. |
| `FORGE_ASSERT_DEBUG`      | Debug-only assertion, active when debug logging is enabled.|

### Examples

```c
#include "asserts.h"

void exampleAssertions() 
{
    int x = 5;
    FORGE_ASSERT(x > 0); // Passes
    FORGE_ASSERT_MESSAGE(x < 0, "x should be less than 0"); // Fails with a message

    #if LOG_DEBUG_ENABLED == 1
    FORGE_ASSERT_DEBUG(x == 5); // Debug assertion, passes in debug mode
    #endif
}
```

In case of assertion failure, `reportAssertionFailure` logs the failing expression, file, and line number.

---

## File System Operations

The file system module provides functions for working with files, such as checking file existence, reading, writing, and getting file sizes.

### File Operations

| Function               | Description                                      |
|------------------------|--------------------------------------------------|
| `getFileSize`          | Retrieves the size of a file.                   |
| `fileExists`           | Checks if a file exists at a given path.        |
| `openFile`             | Opens a file in a given mode (read, write, etc.). |
| `closeFile`            | Closes an open file.                            |
| `readFileLine`         | Reads a single line from the file.              |
| `writeFileLine`        | Writes a line of text to the file.              |
| `readFile`             | Reads a specific number of bytes from a file.   |
| `writeFile`            | Writes data to a file.                          |
| `setCursor`            | Moves the file cursor to a specific position.   |
| `offsetCursor`         | Moves the file cursor by a specified offset.    |
| `getCursor`            | Retrieves the current position of the file cursor. |

### Examples

```c
#include "filesystem.h"
#include "logger.h"

void exampleFilesystem() 
{
    File myFile;
    
    if (openFile("test.txt", FILE_MODE_READ, false, &myFile)) 
    {
        char* line;
        while (readFileLine(&myFile, &line)) 
        {
            FORGE_LOG_INFO("Line: %s", line);
        }
        closeFile(&myFile);
    } 
    else 
    {
        FORGE_LOG_ERROR("Failed to open file!");
    }

    if (fileExists("test.txt")) 
    {
        FORGE_LOG_INFO("File exists!");
    }
}
```

---

## Building and Linking

To use this library in your project:

1. Include the header files `logger.h`, `asserts.h`, and `filesystem.h`.
2. Ensure that your project is set up to link with the necessary system libraries (e.g., for file operations, you may need `libc`).
3. Use the macros and functions as shown in the examples to manage logging, assertions, and file I/O in your project.

---

## License

The Forge Utilities Library is open source and available for use under the MIT License. See the LICENSE file for more details.
