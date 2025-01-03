#pragma once
#include "../defines.h"
#include "../include/tokens.h"


// - - - Binary AST Node specific
typedef enum BinaryOperator
{
  BINARY_OPERATOR_ADDITION,
  BINARY_OPERATOR_SUBTRACTION,
  BINARY_OPERATOR_MULTIPLICATION,
  BINARY_OPERATOR_DIVISION,
  BINARY_OPERATOR_REMAINDER,
  BINARY_OPERATOR_POWER
} BinaryOperator;


// - - - AST Node related structs - - - 

// - - - all the types of Nodes in our AST
typedef enum NodeType
{
  NODE_TYPE_NUMBER,                     // - - - 5
  NODE_TYPE_VARIABLE,                   // - - - x
  NODE_TYPE_BINARY_OPERATOR,            // - - - +
  NODE_TYPE_RETURN,                     // - - - return
  NODE_TYPE_ASSIGNMENT,                 // - - - =
  NODE_TYPE_PREFIX,                    // - - - ++x/!x
  NODE_TYPE_COUNT                       // - - - keep a count of all type of nodes
} NodeType;

// - - - context saves the data of an AST Node
typedef union NodeContext
{
  struct 
  {
    i64 value;   
  } numberContext;

  struct 
  {
    struct Node*      left;
    struct Node*      right;
    BinaryOperator    opcode;
  } binaryContext;

  struct 
  {
    const char*       name;
  } variableContext;

  struct 
  {
    const char*       name;
    struct Node*      value;
  } assignmentContext;

  struct 
  {
    struct Node*      value;
  } returnContext;
  
  struct
  {
    const char* operatorType;
    struct Node* right;
  } prefixContext;
  
} NodeContext;

// - - - the mighty ast node
typedef struct Node 
{
  Token        token;
  NodeType      type;
  NodeContext   context;
} Node;


// - - - AST Node related Functions - - - 

// - - - initialize nodes of each type
FORGE_API bool        initNumberNode       (Node* NODE, u64 VALUE);

FORGE_API bool        initBinaryOpNode     (Node* NODE, Node* LEFT_NODE, Node* RIGHT_NODE, BinaryOperator OPCODE);

FORGE_API bool        initVariableNode     (Node* NODE, std::string& NAME);

FORGE_API bool        initAssignmentNode   (Node* NODE, std::string& NAME, Node* VALUE);

FORGE_API bool       initReturnNode       (Node* NODE, Node* VALUE);

FORGE_API bool      initPrefixNode       (Node* NODE, const char* OPERATOR, Node* RIGHT);
// - - - print a node
FORGE_API std::string getNodeTypeString (NodeType TYPE);

FORGE_API std::string getNodeString     (Node* NODE);

FORGE_API std::string getBinaryOperatorString(BinaryOperator TYPE);
