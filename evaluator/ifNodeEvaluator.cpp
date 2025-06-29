#include "../include/evaluator.h"
#include "../include/program.h"


Data evaluateIfNode(const Node* IF_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(IF_NODE        != NULL,                  "Cannot evaulate a NULL Assignment Node");
  FORGE_ASSERT_MESSAGE(IF_NODE->type  == NODE_TYPE_IF,          "A Node which is not a NODE_TYPE_IF_STATEMENT cannot be evaluated here");

  // - - - check the condition and whether or not it is true
  const Node*   conditional     = IF_NODE->context.ifContext.condition;
  const Block*  alternative     = IF_NODE->context.ifContext.alternative;
  const Block*  consequence     = IF_NODE->context.ifContext.consequence;

  Data execute = evaluate(conditional, NULL);
  if (execute.type != Bool) raiseException("The condition of an if statement must be of type : Bool");
  
  bool decision = *((bool*) execute.memory);
  Data data;
  data.type = Null;


  if (decision)
  {
    if (consequence == NULL)                    return data;
    for (Node* it : consequence->statements)    evaluate(it, NULL);
  }
  else 
  {
    if (alternative == NULL)                    return data;
    for (Node* it : alternative->statements)    evaluate(it, NULL);
  }
  return data;
}
