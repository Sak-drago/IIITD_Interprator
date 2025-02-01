#pragma once 
#include "parser.h"
#include "program.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <limits>


// - - - Structs and Enums - - - 

// - - - @brief : All the possible types, we would support
typedef enum DataType
{
  // - - - signed integers
  Int_8,
  Int_16,
  Int_32,
  Int_64,
  // - - - floating point numbers
  Float_32,
  Float_64,
  // - - - unsigned integers
  UInt_8,
  UInt_16,
  UInt_32,
  UInt_64,
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



// - - - Functions - - - 

// - - - @brief : returns a string representation of the Data Type
FORGE_API std::string   getDataTypeStr          (const DataType DATA_TYPE);

// - - - @brief : returns a string representation of the Data
FORGE_API std::string   getDataStr              (const Data* DATA);

// - - - @brief : returns how many bytes are needed to store the Data Type
FORGE_API i8            getDataTypeSize         (const DataType DATA_TYPE);

// - - - @brief : construct Data out of the AST and raises Exceptions
FORGE_API Data          evaluate                (const Node* NODE);

// - - - @brief : these functions evaluate a specific Node Type
FORGE_API Data          evaluateNumberNode      (const Node* NUMBER_NODE);
FORGE_API Data          evaluateVariableNode    (const Node* VARIABLE_NODE);
FORGE_API Data          evaluateBinaryNode      (const Node* BINARY_NODE);
FORGE_API Data          evaluateReturn          (const Node* RETURN_NODE);
FORGE_API Data          evaluateAssignmentNode  (const Node* ASSIGNMENT_NODE);
FORGE_API Data          evaluatePrefixNode      (const Node* PREFIX_NODE);
FORGE_API Data          evaluateBooleanNode     (const Node* BOOLEAN_NODE);
FORGE_API Data          evaluatePrefixNode      (const Node* PREFIX_NODE);
FORGE_API Data          evaluateIfNode          (const Node* IF_NODE);
FORGE_API Data          evaluateFunctionNode    (const Node* FUNCTION_NODE);

// WARN:- - - @brief : not fully fleshed out yet
FORGE_API void          raiseException(const char* EXCEPTION);
