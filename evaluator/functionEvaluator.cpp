#include "../include/evaluator.h"
#include "../library/include/logger.h"
#include "../library/include/asserts.h"

Data evaluateFunctionNode(const Node* FUNCTION_NODE)
{
  FORGE_ASSERT_MESSAGE(FUNCTION_NODE != NULL, "Cannot evaluate a NULL Function Node");
  FORGE_ASSERT_MESSAGE(FUNCTION_NODE->type == NODE_TYPE_FUNCTION, "Can only evaluate FUNCTION NODES here");

  std::string functionName = FUNCTION_NODE->context.functionContext.name;
  
}
