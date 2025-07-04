#include "../include/ast.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <string>
#include <sstream>

// - - - Helper Functions - - -

static int binaryNodecount = 0;
// - - - Get the unique ID of a node
std::string getNodeID(Node* node) {
    std::ostringstream oss;
    oss << node; // Use the pointer address as a unique ID
    return oss.str();
}


// - - - Initialize all node types - - -

// - - - number nodes
bool initNumberNode(Node* NODE, std::string& NUMBER)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Number Node");
  
  NODE->type                        = NODE_TYPE_NUMBER;
  NODE->context.numberContext.value = strdup(NUMBER.c_str());

  return true;
}


// - - -  initialize a boolean node (true/false)
bool initBoolNode(Node* NODE, bool VALUE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Boolean Node");

  NODE->type                        = NODE_TYPE_BOOLEAN;
  NODE->context.booleanContext.value = VALUE;

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
bool initAssignmentNode(Node* NODE, std::string& NAME, Node* VALUE, bool IS_PLAG)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Assignment Node");

  if (VALUE == NULL)
  {
    FORGE_LOG_WARNING("The Node passed as VALUE for AST Assignemnt Node intiailization is null");
  }

  NODE->type                             = NODE_TYPE_ASSIGNMENT;
  NODE->context.assignmentContext.isPlag = IS_PLAG;
  NODE->context.assignmentContext.name   = strdup(NAME.c_str());
  NODE->context.assignmentContext.value  = VALUE;

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

// - - - initialize an if node
bool initIfNode(Node* NODE, Node* CONDITION, Block* CONSEQUENCE, Block* ALTERNATIVE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST If Node");

  if (CONDITION   == NULL)  FORGE_LOG_ERROR   ("If Node Condition is NULL");
  if (CONSEQUENCE == NULL)  FORGE_LOG_ERROR   ("If Node Consequence is NULL");
  if (ALTERNATIVE == NULL)  FORGE_LOG_WARNING ("If Node's ELSE is NULL");

  NODE->type                          = NODE_TYPE_IF;
  NODE->context.ifContext.condition   = CONDITION;
  NODE->context.ifContext.consequence = CONSEQUENCE;
  NODE->context.ifContext.alternative = ALTERNATIVE;

  return true;
}

// - - - initialize an Fn node
bool initFunctionNode(Node* NODE, const char* NAME, std::vector<FunctionParameter> PARAMETERS, Block* BODY)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot initialize a NULL AST Function Node");

  if (BODY == NULL)
  {
    FORGE_LOG_WARNING("The Node passed as BODY for AST Function Node intiailization is null");
  }

  NODE->type                          = NODE_TYPE_FUNCTION;
  NODE->context.functionContext.name  = strdup(NAME);
  NODE->context.functionContext.parameters = PARAMETERS;
  NODE->context.functionContext.body       = BODY;

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
    case NODE_TYPE_BOOLEAN          : return "NODE_TYPE_BOOLEAN";
    case NODE_TYPE_IF               : return "NODE_TYPE_IF";
    case NODE_TYPE_PREFIX           : return "NODE_TYPE_PREFIX";
    case NODE_TYPE_FUNCTION         : return "NODE_TYPE_FUNCTION";

    default                         : FORGE_ASSERT_MESSAGE(true, "Tu yahan tak aaya kaise bhai. Agar likha hai maine ki it is to keep a count of all types of variables. It is not a type of Node itself.");
  }

  return nullptr; // - - - fuck you - - - No
}

std::string getBinaryOperatorString(BinaryOperator TYPE)
{
  switch (TYPE)
  {
    case BINARY_OPERATOR_ADDITION             : return "BINARY_OPERATOR_ADDITION(+)";
    case BINARY_OPERATOR_SUBTRACTION          : return "BINARY_OPERATOR_SUBTRACTION(-)";
    case BINARY_OPERATOR_MULTIPLICATION       : return "BINARY_OPERATOR_MULTIPLICATION(*)";
    case BINARY_OPERATOR_DIVISION             : return "BINARY_OPERATOR_DIVISION(/)";
    case BINARY_OPERATOR_REMAINDER            : return "BINARY_OPERATOR_REMAINDER(%)";
    case BINARY_OPERATOR_POWER                : return "BINARY_OPERATOR_POWER(^)";

    // - - - Comparison Operator
    case COMPARISON_OPERATOR_EQUAL            : return "COMPARISON_OPERATOR_EQUAL(==)";
    case COMPARISON_OPERATOR_GREATER          : return "COMPARISON_OPERATOR_GREATER(>)";
    case COMPARISON_OPERATOR_LESSER           : return "COMPARISON_OPERATOR_LESSER(<)";
    case COMPARISON_OPERATOR_GREATER_EQUAL    : return "COMPARISON_OPERATOR_GREATER_EQUAL(=>)";
    case COMPARISON_OPERATOR_LESSER_EQUAL     : return "COMPARISON_OPERATOR_LESSER_EQUAL(<=)";
  }
}

std::string getBlockString(std::vector<Node*> statements)
{
    std::string retVal = "{ ";
    for (Node* node : statements)    retVal += getNodeString(node);
    return retVal + " }";
}

std::string getNodeString(Node* NODE, i8 INDENTATION_LEVEL)
{
    if (NODE == NULL)
    {
        FORGE_LOG_WARNING("Printing a NULL Node");
        return "{ NULL }";
    }

    // - - - Maintain a map to track visited nodes
    static std::unordered_map<Node*, std::string> visitedNodes;

    // - - - Assign a unique ID to the node for future reference
    std::string nodeID  = "NODE_" + getNodeID(NODE);
    visitedNodes[NODE]  = nodeID;

    // - - - Keep track of strings needed to make the print more human readable
    std::string indent  = std::string(INDENTATION_LEVEL, '\t');
    std::string retVal  = "{ " + nodeID + " | ";
    retVal              += getNodeTypeString(NODE->type);
    retVal              += " \t| Context : ";

    switch (NODE->type)
    {
        case NODE_TYPE_NUMBER:
            retVal += "\tValue : "                        + std::string(NODE->context.numberContext.value);
            break;

        case NODE_TYPE_VARIABLE:
            retVal += "\tName : "                         + std::string(NODE->context.variableContext.name);
            break;

        case NODE_TYPE_BINARY_OPERATOR:
            retVal += "\n\t" + indent + "Left      : "      + getNodeString(NODE->context.binaryContext.left,  INDENTATION_LEVEL + 1);
            retVal += "\n\t" + indent + "Right     : "      + getNodeString(NODE->context.binaryContext.right, INDENTATION_LEVEL + 1);
            retVal += "\n\t" + indent + "Operation : "      + getBinaryOperatorString(NODE->context.binaryContext.opcode);
            break;

        case NODE_TYPE_ASSIGNMENT:
            retVal += "\tName : "                         + std::string(NODE->context.assignmentContext.name);
            retVal += "\n\t" + indent + "Value : "          + getNodeString(NODE->context.assignmentContext.value, INDENTATION_LEVEL + 1);
            retVal += "\n\t" + indent + "is Plag : "          + (NODE->context.assignmentContext.isPlag ? "Plag" : "Normal");
            break;

        case NODE_TYPE_RETURN:
            retVal += "\n\t" + indent + "Value : "          + getNodeString(NODE->context.returnContext.value, INDENTATION_LEVEL + 1);
            break;

        case NODE_TYPE_PREFIX:
            retVal += "\n\tOperator : "                   + std::string(NODE->context.prefixContext.operatorType);
            retVal += "\n\t" + indent + "Right : "          + getNodeString(NODE->context.prefixContext.right, INDENTATION_LEVEL + 1);
            break;

        case NODE_TYPE_BOOLEAN:
            retVal += "\tValue : "                        + std::to_string(NODE->context.booleanContext.value);
            break;

        case NODE_TYPE_IF:
            retVal += "\n\t" + indent + "Condition : "      + getNodeString(NODE->context.ifContext.condition, INDENTATION_LEVEL + 1);
            retVal += "\n\tConsequence : "                + getBlockString(NODE->context.ifContext.consequence->statements);
            if (NODE->context.ifContext.alternative && NODE->context.ifContext.alternative->statements.size()>0) 
            {
              retVal += "\n\tAlternative : "              + getBlockString(NODE->context.ifContext.alternative->statements);
            }
            break;

        case NODE_TYPE_FUNCTION:
            retVal += "\n\tName : "                         + std::string(NODE->context.functionContext.name);
            retVal += "\n\tParameters : ";
            for (u64 i = 0; i < NODE->context.functionContext.parameters.size(); i++)
            {
                FORGE_LOG_DEBUG("Current parameter : %s", NODE->context.functionContext.parameters[i].name);
                retVal += "\n\t\t" + std::string(NODE->context.functionContext.parameters[i].name);
            }
            if(NODE->context.functionContext.body!= nullptr) 
            { 
              retVal += "\n\tBody : " + getBlockString(NODE->context.functionContext.body->statements);
            }
            break;

        default:
            FORGE_ASSERT_MESSAGE(true, "Invalid Node type encountered.");
    }

    return retVal + " }";
}
