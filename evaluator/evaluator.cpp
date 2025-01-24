#include "../include/object.h"
#include "../library/include/asserts.h"


// - - - All the eval functions - - -

// - - - number node
Object evaluateNumberNode(Node* NUMBER_NODE);
// - - - 


// - - - evaluate the Node - - - 

Object evaluate(Node* NODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");

  switch (NODE->type)
  {
    NODE_TYPE_NUMBER : return evaluateNumberNode(NODE);
  }
}

Object evaulateNumberNode(Node* NUMBER_NODE)
{
  FORGE_ASSERT_MESSAGE(NUMBER_NODE        != NULL,              "Cannot evaulate a NULL Number Node");
  FORGE_ASSERT_MESSAGE(NUMBER_NODE->type  != NODE_TYPE_NUMBER,  "A Node which is not a NODE_TYPE_NUMBER cannot be evaluated here");

  return Object 
  {
    .type   = ObjectType::Int_8,
    .value  = NULL
  };
}
