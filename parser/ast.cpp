#include "../include/ast.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <string>
#include <sstream>

// - - - Helper Functions - - -

// - - - Get the unique ID of a node
std::string getNodeID(Node* node) {
    std::ostringstream oss;
    oss << node; // Use the pointer address as a unique ID
    return oss.str();
}

// - - - Initialize all node types - - - 
// - - - number nodes
bool initNumberNode(Node* NODE, u64 VALUE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Number Node");
  
  NODE->type                        = NODE_TYPE_NUMBER;
  FORGE_LOG_TRACE("%d, value receieved for %p", VALUE, NODE);
  NODE->context.numberContext.value = VALUE;

  return true;
}

// - - - initialize a binary opeartor node 
bool initBinaryOpNode(Node* NODE, Node* LEFT_NODE, Node* RIGHT_NODE, BinaryOperator OPCODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Binary Operation Node");

  NODE->type                            = NODE_TYPE_BINARY_OPERATOR;

  if (LEFT_NODE == NULL)
  {
    FORGE_LOG_ERROR("Left Node is NULL. Failed to intialize");
    return false;
  }

  if (RIGHT_NODE == NULL)
  {
    FORGE_LOG_ERROR("Right Node is NULL. Failed to intialize");
    return false;
  }
  
  NODE->context.binaryContext.left      = LEFT_NODE;
  NODE->context.binaryContext.right     = RIGHT_NODE;
  NODE->context.binaryContext.opcode    = OPCODE;

  return true;
}

// - - - initialize a variable node 
bool initVariableNode(Node* NODE, std::string& NAME)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Variable Node");

  // - - - TODO @Sakshat : Not sure, but maybe you might want to check for a keyword use a variable name syntax error somewhere  
  
  // - - - TODO - Check if variable has not been already initalized.
  
  NODE->type                            = NODE_TYPE_VARIABLE;
  NODE->context.variableContext.name    = strdup(NAME.c_str());

  return true;
}

// - - - initiliaze an assignment node
bool initAssignmentNode(Node* NODE, std::string& NAME, Node* VALUE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Assignment Node");

  if (VALUE == NULL)
  {
    FORGE_LOG_WARNING("The Node passed as VALUE for AST Assignemnt Node intiailization is null");
  }

  NODE->type                            = NODE_TYPE_ASSIGNMENT;
  NODE->context.assignmentContext.name  = strdup(NAME.c_str());
  NODE->context.assignmentContext.value = VALUE;

  return true;
}

// - - - initialize a return node
bool initReturnNode(Node* NODE, Node* VALUE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Return Node");

  if (VALUE == NULL)
  {
    FORGE_LOG_WARNING("The Node passed as VALUE for AST Return Node intiailization is null");
  }

  NODE->type                          = NODE_TYPE_RETURN;
  NODE->context.returnContext.value   = VALUE;

  return true;
}


// - - - initialize a prefix node
bool initPrefixNode(Node* NODE, const char* OPERATOR, Node* RIGHT)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Prefix Node");

  if (RIGHT == NULL)
  {
    FORGE_LOG_WARNING("The Node passed as RIGHT for AST Prefix Node intiailization is null");
  }

  NODE->type                          = NODE_TYPE_PREFIX;
  NODE->context.prefixContext.operatorType = strdup(OPERATOR);
  NODE->context.prefixContext.right        = RIGHT;

  return true;
}
// - - - Print Nodes - - - 

std::string getNodeTypeString(NodeType TYPE)
{
  switch (TYPE)
  {
    case NODE_TYPE_ASSIGNMENT       : return "NODE_TYPE_ASSIGNMENT";
    case NODE_TYPE_VARIABLE         : return "NODE_TYPE_VARIABLE";
    case NODE_TYPE_NUMBER           : return "NODE_TYPE_NUMBER";
    case NODE_TYPE_BINARY_OPERATOR  : return "NODE_TYPE_BINARY_OPERATOR";
    case NODE_TYPE_RETURN           : return "NODE_TYPE_RETURN";
    case NODE_TYPE_PREFIX           : return "NODE_TYPE_PREFIX";

    default:
      FORGE_ASSERT_MESSAGE(true, "Tu yahan tak aaya kaise bhai. Agar likha hai maine ki it is to keep a count of all types of variables. It is not a type of Node itself.");
  }

  return nullptr; // - - - fuck you - - - No
}

std::string getBinaryOperatorString(BinaryOperator TYPE)
{
  switch (TYPE)
  {
    case BINARY_OPERATOR_ADDITION       : return "BINARY_OPERATOR_ADDITION(+)";
    case BINARY_OPERATOR_SUBTRACTION    : return "BINARY_OPERATOR_SUBTRACTION(-)";
    case BINARY_OPERATOR_MULTIPLICATION : return "BINARY_OPERATOR_MULTIPLICATION(*)";
    case BINARY_OPERATOR_DIVISION       : return "BINARY_OPERATOR_DIVISION(/)";
    case BINARY_OPERATOR_REMAINDER      : return "BINARY_OPERATOR_REMAINDER(%)";
    case BINARY_OPERATOR_POWER          : return "BINARY_OPERATOR_POWER(^)";
  }
}

std::string getNodeString(Node* NODE)
{
    if (NODE == NULL)
    {
        FORGE_LOG_WARNING("Printing a NULL Node");
        return "{ NULL }";
    }

    // Maintain a map to track visited nodes
    static std::unordered_map<Node*, std::string> visitedNodes;

    // Check if the node has already been visited

    // Assign a unique ID to the node for future reference
    std::string nodeID = "NODE_" + getNodeID(NODE);
    visitedNodes[NODE] = nodeID;

    std::string retVal = "{ " + nodeID + " | ";
    retVal += getNodeTypeString(NODE->type);
    retVal += " \t| Context : ";

    switch (NODE->type)
    {
        case NODE_TYPE_NUMBER:
            retVal += "\tValue : " + std::to_string(NODE->context.numberContext.value);
            break;

        case NODE_TYPE_VARIABLE:
            retVal += "\tName : " + std::string(NODE->context.variableContext.name);
            break;

        case NODE_TYPE_BINARY_OPERATOR:
            retVal += "\n\tLeft : " + getNodeString(NODE->context.binaryContext.left);
            retVal += "\n\tRight : " + getNodeString(NODE->context.binaryContext.right);
            retVal += "\n\tOperation : " + getBinaryOperatorString(NODE->context.binaryContext.opcode);
            break;

        case NODE_TYPE_ASSIGNMENT:
            retVal += "\tName : " + std::string(NODE->context.assignmentContext.name);
            retVal += "\nValue : " + getNodeString(NODE->context.assignmentContext.value);
            break;

        case NODE_TYPE_RETURN:
            retVal += "\n\tValue : " + getNodeString(NODE->context.returnContext.value);
            break;

        case NODE_TYPE_PREFIX:
            retVal += "\n\tOperator : " + std::string(NODE->context.prefixContext.operatorType);
            retVal += "\n\tRight : " + getNodeString(NODE->context.prefixContext.right);
            break;

        default:
            FORGE_ASSERT_MESSAGE(true, "Invalid Node type encountered.");
    }

    return retVal + " }";
}