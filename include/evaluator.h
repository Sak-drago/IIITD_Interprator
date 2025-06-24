#pragma once 
#include "program.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/garbageCollector.h"
#include <limits>


// - - - runtime
FORGE_API ExitMessage  run();
bool          createRuntime           ();
void          bol                     (const char* MESSAGE);
Data*         createVariable          (std::string& NAME, DataType TYPE);
// - - - @brief : construct Data out of the AST and raises Exceptions
Data          evaluate                (const Node* NODE, const char* NAME);

// - - - @brief : these functions evaluate a specific Node Type
Data          evaluateNumberNode      (const Node* NUMBER_NODE,     const char* NAME);
Data          evaluateVariableNode    (const Node* VARIABLE_NODE);
Data          evaluateBinaryNode      (const Node* BINARY_NODE,     const char* NAME);
Data          evaluateReturn          (const Node* RETURN_NODE,     const char* NAME);
Data          evaluateAssignmentNode  (const Node* ASSIGNMENT_NODE);
Data          evaluatePrefixNode      (const Node* PREFIX_NODE,     const char* NAME);
Data          evaluateBooleanNode     (const Node* BOOLEAN_NODE,    const char* NAME);
Data          evaluatePrefixNode      (const Node* PREFIX_NODE,     const char* NAME);
Data          evaluateIfNode          (const Node* IF_NODE);
Data          evaluateFunctionNode    (const Node* FUNCTION_NODE,   const char* NAME);

// WARN:- - - @brief : not fully fleshed out yet
void          raiseException(const char* EXCEPTION);
