#include "../include/evaluator.h"
#include "../include/program.h"
#include "../include/ast.h"
#include "../include/garbageCollector.h"
#include <string>



Program runtime;

// - - - evaluate the Node - - - 

Data evaluate(const Node* NODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  /*
  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER                   : return evaluateNumberNode     (NODE);
    case NODE_TYPE_BINARY_OPERATOR          : return evaluateBinaryNode     (NODE);
    case NODE_TYPE_BOOLEAN                  : return evaluateBooleanNode    (NODE);
    case NODE_TYPE_ASSIGNMENT               : return evaluateAssignmentNode (NODE);
    case NODE_TYPE_VARIABLE                 : return evaluateVariableNode   (NODE);
    case NODE_TYPE_IF                       : return evaluateIfNode         (NODE);
    default : raiseException(std::string("Type not handled yet : " + std::string(getNodeString((Node*) NODE).c_str())).c_str());
    exit(1);
  }
*/
}


// - - - IO functions - - - 
void raiseException(const char* EXCEPTION) { FORGE_LOG_ERROR("Exception raised:\t%s", EXCEPTION); }
void say(const char* MESSAGE)
{
  FORGE_ASSERT_MESSAGE(MESSAGE, "Cannot print a NULL Message");
  static u64 written = 0;
  writeFile(&runtime.output, strlen(MESSAGE), MESSAGE, &written);
  FORGE_LOG_INFO(MESSAGE);
}


// - - - | Runtime | - - - 


// - - - setup - - - 

bool createRuntime()
{
  if (!openFile("output.txt", FILE_MODE_WRITE, false, &runtime.output)) return false;
  startGarbageCollector(1);
  say("IIIT-D Language (v1.0.0): by Just Somebody and Sakshat Sachdeva\n");
  return true;
}

ExitMessage run()
{
  ExitMessage failureMode;
  if (!createRuntime())
  {
    failureMode = EXIT_MESSAGE_FILE_CREATION_FAIL;
    goto exit;
  }

  for (Node* n: runtime.statements)    evaluate(n);

exit:
  switch(failureMode)
  {
    case EXIT_MESSAGE_FILE_CREATION_FAIL : 
      FORGE_LOG_FATAL("We failed to create the Runtime for the program to run");
      break;

    default :
      FORGE_LOG_WARNING("Not yet Handled");
      exit(1);
  }
  return failureMode;
}

