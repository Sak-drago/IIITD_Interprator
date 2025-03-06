#pragma once 
#include "../defines.h"
#include "../library/include/linearAlloc.h"
#include "../library/include/filesystem.h"
#include "ast.h"
#include <unordered_map>
#include <vector>


// - - - Structs and Enums - - - 

// - - - @brief : All the ways that the program can fail
typedef enum ExitMessage
{
  EXIT_MESSAGE_SYNTAX_ERROR,
  EXIT_MESSAGE_OUT_OF_MEMORY,
  EXIT_MESSAGE_FILE_CREATION_FAIL,
  EXIT_MESSAGE_SUCCESS,
  EXIT_MESSAGE_COUNT
} ExitMessage;

// - - - @brief : All the possible types, we would support
typedef enum DataType
{
  // - - - Integer, Float
  Integer,
  Float,
  // - - - life permitting data types
  Bool,
  Null,
  String,
  Array,
  List,
  Dictionary,
  // - - - used only by the evaluator
  ERROR,
  DATA_TYPE_COUNT
} DataType;

// - - - @brief : The main object of the Evaluator
typedef struct Data
{
  DataType type;
  void*    value;
} Data;

// - - - @vrief : Environment is : stack heap and so on
typedef struct Environment
{
  LinearAllocator                         memory;
  std::unordered_map<std::string, Data>   pointers;
} Environment;

typedef struct Program
{
  std::vector<Node*>                        statements;
  std::vector<Node*>                        functionDefined;
  std::unordered_map<std::string, Node*>    functionPointers;
  std::unordered_map<std::string, Data>     pointers;
  LinearAllocator                           allocator;
  File                                      output;
} Program;


// - - - Functions - - - 

// - - - @brief : returns a string representation of the Data Type
FORGE_API std::string   getDataTypeStr          (const DataType DATA_TYPE);

// - - - @brief : returns a string representation of the Data
FORGE_API std::string   getDataStr              (const Data* DATA);

// - - - @brief : returns how many bytes are needed to store the Data Type
FORGE_API i8            getDataTypeSize         (const DataType DATA_TYPE);

// - - - @brief : tells whether the currentEnv is a local environment
FORGE_API bool          isCurrentEnvLocal();

// - - - @brief : tells whether the variable is in the local environment
FORGE_API bool          isVariableInLocalEnv   (std::string& VAR_NAME);

// - - - @brief : tells whether the variable is in the global environmnet
FORGE_API bool          isVariableInGlobalEnv  (std::string& VAR_NAME);

// - - - sigleton program
extern Program runtime;
