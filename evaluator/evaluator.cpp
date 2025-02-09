#include "../include/evaluator.h"
#include "../include/program.h"
#include "../include/ast.h" 

Program runtime;


// - - - evaluate the Node - - - 

Data evaluate(const Node* NODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER                   : return evaluateNumberNode     (NODE);
    case NODE_TYPE_BINARY_OPERATOR          : return evaluateBinaryNode     (NODE);
    case NODE_TYPE_BOOLEAN                  : return evaluateBooleanNode    (NODE);
    case NODE_TYPE_ASSIGNMENT               : return evaluateAssignmentNode (NODE);
    case NODE_TYPE_VARIABLE                 : return evaluateVariableNode   (NODE);
    default : raiseException(std::string("Type not handled yet : " + std::string(getNodeString((Node*) NODE).c_str())).c_str());
  }
}

void raiseException(const char* EXCEPTION) { FORGE_LOG_ERROR("Exception raised:\t%s", EXCEPTION); }
