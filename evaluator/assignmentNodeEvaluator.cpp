#include "../include/evaluator.h"
#include "../include/program.h"
#include <unordered_map>

/*

Data evaluateAssignmentNode(const Node* ASSIGNMENT_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE->type  == NODE_TYPE_ASSIGNMENT,  "A Node which is not a NODE_TYPE_ASSIGNMENT cannot be evaluated here");
 
  std::string name      = ASSIGNMENT_NODE->context.assignmentContext.name;

  /* - - - check if the variable exists already, 
   * first check local and then check global
    and if so, get the size of the variable and decide whether we can assign to it         

    if it doesnt exist, assign it to the local environment

  std::unordered_map<std::string, Data>* pointers = &runtime.currentEnv->pointers; 
  Data      data;
  DataType  type = DATA_TYPE_COUNT;
  if (isVariableInLocalEnv(name))
  {
    FORGE_LOG_TRACE("Found %s in Local environment", name.c_str());
    pointers  = &runtime.stack.pointers;
    data      = pointers->find(name)->second;
    type      = data.type;
    goto reassign;
  }
  if (isVariableInGlobalEnv(name))
  {
    FORGE_LOG_TRACE("Found %s in Global environment", name.c_str());
    pointers  = &runtime.global.pointers;
    data      = pointers->find(name)->second;
    type      = DATA_TYPE_COUNT;
    goto reassign;
  }

  reassign:
    Data secondary    = evaluate(ASSIGNMENT_NODE->context.assignmentContext.value);
    if (type != DATA_TYPE_COUNT && secondary.type != data.type) 
    {
      FORGE_LOG_WARNING("Chaning type of %s from %s to %s", 
                        name.c_str(), 
                        getDataTypeStr(data.type).c_str(), 
                        getDataTypeStr(secondary.type).c_str());
    }
    data              = secondary;
    // - - - TODO: Remove this after the parser errors are fixed:
    name = "x";
    (*pointers)[name] = data;
    return (*pointers)[name];
}
*/
