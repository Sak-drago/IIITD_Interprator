#include "../include/evaluator.h"
#include "../include/program.h"
#include <cstring>
#include <unordered_map>


Data evaluateAssignmentNode(const Node* ASSIGNMENT_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE->type  == NODE_TYPE_ASSIGNMENT,  "A Node which is not a NODE_TYPE_ASSIGNMENT cannot be evaluated here");
 
  // - - - check if the variable exists already, 
  std::string name      = ASSIGNMENT_NODE->context.assignmentContext.name;
  bool        toPlag    = ASSIGNMENT_NODE->context.assignmentContext.isPlag; 
  Node*       rhs       = ASSIGNMENT_NODE->context.assignmentContext.value;
  auto        that      = runtime.variables.find(name);
  bool        exists    = that != runtime.variables.end();

  if (!exists)
  {
    // - - - create a new variable
    if (toPlag)
    {
      if (rhs->type != NODE_TYPE_VARIABLE) { raiseException("Can only plag a variable, cannot plag say an Integer"); }
      Data  rhsData     = evaluateVariableNode(ASSIGNMENT_NODE->context.assignmentContext.value, NULL); 
      Data* pointer     = createVariable(name, Plag);
      void* memory      = pointer->memory; 
      Data* actual;
      if (rhsData.type == Plag) 
      { 
        actual = (Data*) rhsData.memory;
        actual->refCount++;
      }
      else 
      {
        actual; 
      }
    }
  }
}
