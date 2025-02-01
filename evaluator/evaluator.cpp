#include "../include/evaluator.h"

Program runtime;


// - - - evaluate the Node - - - 

Data evaluate(const Node* NODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER                   : return evaluateNumberNode(NODE);
    case NODE_TYPE_BINARY_OPERATOR          : return evaluateBinaryNode(NODE);
    case NODE_TYPE_BOOLEAN                  : return evaluateBooleanNode(NODE);
  }
}

void raiseException(const char* EXCEPTION)
{
  FORGE_LOG_ERROR("Exception raised:\t%s", EXCEPTION);
}



