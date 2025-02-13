#include "../include/evaluator.h"
#include "../include/program.h"


Data evaluatePrefixNode(const Node* PREFIX_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(PREFIX_NODE        != NULL,                  "Cannot evaulate a NULL Prefix Node");
  FORGE_ASSERT_MESSAGE(PREFIX_NODE->type  == NODE_TYPE_PREFIX,      "A Node which is not a NODE_TYPE_IF_PREFIX cannot be evaluated here");

  const char* prefix = PREFIX_NODE->context.prefixContext.operatorType;

}
