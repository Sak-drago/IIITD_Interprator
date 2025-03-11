#include "../include/evaluator.h"
#include "../include/program.h"
#include <unordered_map>


Data evaluateVariableNode(const Node* VARIABLE_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(VARIABLE_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(VARIABLE_NODE->type  == NODE_TYPE_VARIABLE,    "A Node which is not a NODE_TYPE_VARIABLE cannot be evaluated here");

  auto that = runtime.variables.find(VARIABLE_NODE->context.variableContext.name);
  if (that == runtime.variables.end()) raiseException("variable doesnt exist");
  return that->second;
}
