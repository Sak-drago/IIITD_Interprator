#include "parserTests.h"
#include "../include/parser.h"
#include "../include/tokenizer.h"


// - - - Common program for all the tests - - - 


static Program program;
std::string customString;

void declareTestCase(std::string STRING)
{
    customString = STRING;
    FORGE_LOG_DEBUG("Working on the testcase : %s", customString.c_str());
}

i32 getNodeCount()
{
  return program.allocator.allocated / sizeof(Node);
}

void printProgram()
{
  FORGE_LOG_INFO("Printing the program generated :-");
  FORGE_LOG_TRACE("Program had %d Nodes in %d statements", getNodeCount(), program.statements.size());  
  for (Node* node : program.statements)
  {
    FORGE_LOG_DEBUG(getNodeString(node).c_str());
  }
}

void readyProgram()
{
  static bool done = false;
  if (!done)
  {
    std::vector<Node*> statements;
    program.statements = statements;
    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    done = true;
    return;
  }
  linearAllocFree(&program.allocator);    
  program.statements.clear();  
}

u8 runTestCase(std::string TEST_CASE, i32 EXPECTED_NODE_COUNT)
{
  readyProgram(); 
  declareTestCase(TEST_CASE);
  std::vector<Token> tokens = tokenize(customString.c_str());
  produceAST(&tokens, &program);
  printProgram();
  expectShouldBe(EXPECTED_NODE_COUNT, getNodeCount());
  FORGE_LOG_DEBUG("- - - Test Case End - - -\n");
  return true;
}


// - - - The tests - - -

// - - - Check Assignment Statements
u8 checkParserAssignment()
{
  if (!runTestCase("Plag myVar = 2", 2))            return false;
  if (!runTestCase("Plag myVar = (1+2)", 4))        return false;  
  return true;
}

// - - - Check Return Statements
u8 checkParserReturn()
{
  if (!runTestCase("return (1)", 2))                return false;
  if (!runTestCase("return 1 + 2", 4))              return false;
  return true;
}

// - - - Check Expression Statements
u8 checkParserExpression()
{
  if (!runTestCase("1+2-3", 5))                     return false;
  if (!runTestCase("1+2*3/4", 7))                   return false;
  if (!runTestCase("1 > 2", 3))                     return false;
  return true;
}

// - - - Check If Block
u8 checkParserIfBlock()
{ 
  if (!runTestCase("agar (1 > 2) { return 1 }", 6))                   return false;
  if (!runTestCase("agar (1 > 2) { return 1 } ya { return 2 }", 8))   return false;
  return 1;
}

// - - - Check Function Block - - -
u8 checkParserFunctionBlock()
{
  if (!runTestCase("Fn() { return 1 }", 3))       return false;
  if (!runTestCase("Fn(x, y) {return 1 + 2}", 7)) return false;
  return true;
}

int main()
{
    registerTest(checkParserAssignment,     "Check if we can parse assignment statements");
    registerTest(checkParserReturn,         "Check if we can parse return statements");
    registerTest(checkParserExpression,     "Check if we can parse expression statements");
    registerTest(checkParserIfBlock,        "Check if we can parse if blocks");
    registerTest(checkParserFunctionBlock,  "Check if we can parse function blocks");
    runTests();
    return 0;
}
