#include "../include/evaluator.h"

Data evaluateBooleanNode(const Node* BOOLEAN_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(BOOLEAN_NODE        != NULL,               "Cannot evaulate a NULL Boolean Node");
  FORGE_ASSERT_MESSAGE(BOOLEAN_NODE->type  == NODE_TYPE_BOOLEAN,  "A Node which is not a NODE_TYPE_BOOLEAN cannot be evaluated here");

  // - - - assign the value to the memory
  void* memory      = linearAllocatorAllocate(&(runtime.currentEnv->memory), getDataTypeSize(Bool)); 
  *((bool*) memory) = BOOLEAN_NODE->context.booleanContext.value;

  // - - - return the data 
  return Data 
    {
      .type  = Bool,
      .value = memory
    };
}
