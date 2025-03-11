#include "../include/evaluator.h"

static i32 anonymousVarCount = 0;

Data evaluateBooleanNode(const Node* BOOLEAN_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(BOOLEAN_NODE        != NULL,               "Cannot evaulate a NULL Boolean Node");
  FORGE_ASSERT_MESSAGE(BOOLEAN_NODE->type  == NODE_TYPE_BOOLEAN,  "A Node which is not a NODE_TYPE_BOOLEAN cannot be evaluated here");

  // - - - assign the value to the memory
  std::string name  = "anonymous_boolean_" + std::to_string(anonymousVarCount++);
  Data* pointer     = createVariable(name, Bool);
  void* memory      = pointer->memory; 
  *((bool*) memory) = BOOLEAN_NODE->context.booleanContext.value;

  return *pointer;
}
