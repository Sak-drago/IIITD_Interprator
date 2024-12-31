#include "../include/ast.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <string>


// - - - Initialize all node types - - - 

// - - - number nodes
bool initNumberNode(Node* NODE, u64 VALUE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Number Node");
  
  NODE->type                        = NODE_TYPE_NUMBER;
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

  return false;
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


// - - - Print Nodes - - - 

std::string getNodeTypeString(NodeType TYPE)
{
  switch (TYPE)
  {
    case NODE_TYPE_ASSIGNMENT       : return "NODE_TYPE_ASSIGNMENT";
    case NODE_TYPE_VARIABLE         : return "NODE_TYPE_VARIABLE";
    case NODE_TYPE_NUMBER           : return "NODE_TYPE_NUMBER";
    case NODE_TYPE_BINARY_OPERATOR  : return "NODE_TYPE_ASSIGNMENT";

    default:
      FORGE_ASSERT_MESSAGE(true, "Tu yahan tak aaya kaise bhai. Agar likha hai maine ki it is to keep a count of all types of variables. It is not a type of Node itself.");
  }

  return nullptr; // - - - fuck you
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
    case BINARY_OPERATOR_POWER          : return "BINARY_OPERATOR_ADDITION(^)";
  }
}

std::string getNodeString (Node* NODE)
{
  if (NODE == NULL)
  {
    FORGE_LOG_WARNING("Printing a NULL Node");
    return "{ NULL }";
  }

  // - - - TODO: @Sakshat : make this code pretty
  std::string retVal  = "{ ";
  retVal              += getNodeTypeString(NODE->type);
  retVal              += " \t| Context : ";

  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER:
      retVal += "Value : " + std::to_string(NODE->context.numberContext.value);
      break;

    case NODE_TYPE_VARIABLE:
      retVal += "Name : " + std::string(NODE->context.variableContext.name);
      break;

    case NODE_TYPE_BINARY_OPERATOR:
      // - - - WARNING:  Hopefylly this isnt an infinite recursion
      retVal += "\n\tLeft : "       + getNodeString(NODE->context.binaryContext.left);
      retVal += "\n\tRight : "      + getNodeString(NODE->context.binaryContext.right);
      retVal += "\n\tOperation : "  + getBinaryOperatorString(NODE->context.binaryContext.opcode);
      break;
      
    case NODE_TYPE_ASSIGNMENT:
      // - - - WARNING: Another potential infinite recursion 
      retVal += "Name : "           + std::string(NODE->context.assignmentContext.name);
      retVal += "\nValue : "        + getNodeString(NODE->context.assignmentContext.value);
      break;

    default:
      FORGE_ASSERT_MESSAGE(true, "Ek baar me samajh nahi aata kya? Tu yahan tak aaya kaise bhai. Agar likha hai maine ki it is to keep a count of all types of variables. It is not a type of Node itself.");
  }
  return retVal + " }";
}
