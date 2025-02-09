#include "../include/evaluator.h"
#include "../include/program.h"
#include <unordered_map>


Data evaluateVariableNode(const Node* ASSIGNMENT_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE->type  == NODE_TYPE_VARIABLE,    "A Node which is not a NODE_TYPE_VARIABLE cannot be evaluated here");
 
  std::string                            name      = ASSIGNMENT_NODE->context.variableContext.name;
  std::unordered_map<std::string, Data>* pointers = &runtime.currentEnv->pointers; 

  if (isVariableInLocalEnv(name))
  {
    FORGE_LOG_TRACE("Found %s in Local environment", name.c_str());
    pointers  = &runtime.stack.pointers;
  }
  else if (isVariableInGlobalEnv(name))
  {
    FORGE_LOG_TRACE("Found %s in Global environment", name.c_str());
    pointers  = &runtime.global.pointers;
  }
  else 
  {
    Data data;
    data.type = Null;
    return data;
  }
  return pointers->find(name)->second;
}
