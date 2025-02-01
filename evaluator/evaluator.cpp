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
  }
}

void raiseException()
{
  FORGE_LOG_ERROR("Exception raised");
}



