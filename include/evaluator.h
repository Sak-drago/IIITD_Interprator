#pragma once 
#include "program.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <limits>



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
