#include "../include/evaluator.h"
#include "../include/program.h"
#include "../include/ast.h"
#include "../include/garbageCollector.h"
#include <string>



Program runtime;

// - - - evaluate the Node - - - 

Data evaluate(const Node* NODE, const char* NAME)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  switch (NODE->type)
  {
    case NODE_TYPE_NUMBER                   : return evaluateNumberNode     (NODE, NAME);
    case NODE_TYPE_BINARY_OPERATOR          : return evaluateBinaryNode     (NODE, NAME);
    case NODE_TYPE_BOOLEAN                  : return evaluateBooleanNode    (NODE, NAME);
//    case NODE_TYPE_ASSIGNMENT               : return evaluateAssignmentNode (NODE, NAME);
    case NODE_TYPE_VARIABLE                 : return evaluateVariableNode   (NODE);
    case NODE_TYPE_IF                       : return evaluateIfNode         (NODE);
    case NODE_TYPE_FUNCTION                 : return evaluateFunctionNode(NODE);
    default : raiseException(std::string("Type not handled yet : " + std::string(getNodeString((Node*) NODE).c_str())).c_str());
    exit(1);
  }
}


// - - - IO functions - - -

void raiseException(const char* EXCEPTION) { FORGE_LOG_ERROR("Exception raised:\t%s", EXCEPTION); }
void bol(const char* MESSAGE)
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

  runtime.variables.clear();
  createLinearAllocator(1024 * 8, 0, NULL, &runtime.stack);


  // - - - insert the functions 
  bol("IIIT-D Language (v1.0.0): by Just Somebody and Sakshat Sachdeva\n\n\n");


  return true;
}

ExitMessage run()
{
  ExitMessage failureMode = EXIT_MESSAGE_SUCCESS;
  if (!createRuntime())
  {
    failureMode = EXIT_MESSAGE_FILE_CREATION_FAIL;
    goto exit;
  }

  for (Node* n: runtime.statements)    evaluate(n, NULL);

exit:
  switch(failureMode)
  {
    case EXIT_MESSAGE_FILE_CREATION_FAIL : 
      FORGE_LOG_FATAL("We failed to create the Runtime for the program to run");
      break;

    case EXIT_MESSAGE_SUCCESS : 
      FORGE_LOG_INFO("Program successfully finished");
      break;

    default :
      FORGE_LOG_WARNING("Not yet Handled");
      exit(1);
  }
  return failureMode;
}


// - - - create varibale
Data* createVariable(std::string& NAME, DataType TYPE)
{
  FORGE_ASSERT_MESSAGE(runtime.variables.find(NAME) == runtime.variables.end(), "Variable already exists");
  FORGE_ASSERT_MESSAGE(TYPE < DATA_TYPE_COUNT, "Unlawful data types");

  runtime.variables[NAME]       = 
                                {
                                  .type     = TYPE,
                                  .memory   = forgeAllocate(getDataTypeSize(TYPE)), 
                                  .refCount = 1,
                                };
  Data* pointer                 = &(runtime.variables[NAME]);
  Data** area                   = (Data**) linearAllocatorAllocate(&runtime.stack, 8);
  *area                         = pointer;

  return pointer;
}
