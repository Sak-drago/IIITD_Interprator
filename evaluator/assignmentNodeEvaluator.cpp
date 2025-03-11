#include "../include/evaluator.h"
#include "../include/program.h"
#include <unordered_map>


Data evaluateAssignmentNode(const Node* ASSIGNMENT_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(ASSIGNMENT_NODE->type  == NODE_TYPE_ASSIGNMENT,  "A Node which is not a NODE_TYPE_ASSIGNMENT cannot be evaluated here");
 
  // - - - check if the variable exists already, 
  std::string name      = ASSIGNMENT_NODE->context.assignmentContext.name;
  bool toPlag           = ASSIGNMENT_NODE->context.assignmentContext.isPlag; 
  auto that             = runtime.variables.find(name);
}
