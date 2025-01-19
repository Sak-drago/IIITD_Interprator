#include "parserTests.h"
#include "../include/parser.h"
#include "../include/tokenizer.h"

static Program program;



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


// - - - Check Assignment Statements - - -
u8 checkParserAssignment()
{
  // - - - For simple assign Statement
  readyProgram();  
  const char* customString = "Plag myVar = 1";
  FORGE_LOG_TRACE("Working on the statement %s", customString);
  std::vector<Token> tokens = tokenize(customString);

  produceAST(&tokens, &program);
  printProgram();  
  expectShouldBe(2, getNodeCount());
    
  FORGE_LOG_DEBUG("- - - \n");
   
  readyProgram();  
  customString = "Plag myVar = (1 + 2)";
  FORGE_LOG_TRACE("Working on the statement %s", customString);
  tokens = tokenize(customString);

  produceAST(&tokens, &program);
  printProgram();  
  expectShouldBe(4, getNodeCount());
  return 1;
}

// - - - Check Return Statements - - -
u8 checkParserReturn()
{
  // - - - For simple return statement
  readyProgram();
  const char* customString = "return (1)";
  FORGE_LOG_TRACE("Working on the statement %s", customString);
  std::vector<Token> tokens = tokenize(customString);

  produceAST(&tokens, &program);
  printProgram();  
  expectShouldBe(2, getNodeCount());  

  FORGE_LOG_DEBUG(" - - - \n");

  readyProgram();  
  customString = "return (1+2)";
  FORGE_LOG_TRACE("Working on the statement %s", customString);
  tokens = tokenize(customString);

  produceAST(&tokens, &program);
  printProgram();  
  expectShouldBe(4, getNodeCount());  

  return 1;
}

// - - - Check Expression Statements - - -
u8 checkParserExpression()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple expression statement
    const char* customString = "1+2-3";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG(" - - - \n");
    exit(1);

    customString = "1+2*3/4";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);\
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "1 > 2";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for(u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();

    FORGE_LOG_INFO("EXPRESSION STATEMENT TESTS COMPLETED");
    return 1;
}

// - - - Check If Block - - -
u8 checkParserIfBlock()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple if block
    const char* customString = "if (1 > 2) { return 1 }";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "if (1 > 2) { return 1 } else { return 2 }";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);

    FORGE_LOG_INFO("IF BLOCK TESTS COMPLETED");
    return 1;
}

// - - - Check Function Block - - -
u8 checkParserFunctionBlock()
{
    Program program;
    std::vector<Node*> statements;
    program.statements = statements;

    // - - - For simple function block
    const char* customString = "Fn() { return 1 }";
    std::vector<Token> tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");

    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);
    program.statements.clear();
    FORGE_LOG_DEBUG("==============================================================");

    customString = "Fn(x,y) { return 1+2 }";
    tokens = tokenize(customString);

    createLinearAllocator(1024 * 1024, 0,  NULL, &program.allocator);
    produceAST(&tokens, &program);

    for (Node* node : program.statements)
    {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    FORGE_LOG_INFO("PRINTING ALL NODES ALLOCATED");


    for (u64 i = 0; i < program.allocator.allocated; i += sizeof(Node))
    {
        Node* node = (Node*)((char*)program.allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
    }

    destroyLinearAllocator(&program.allocator);

    FORGE_LOG_INFO("FUNCTION BLOCK TESTS COMPLETED");
    return 1;
}

int main()
{
    registerTest(checkParserAssignment, "Check if we can parse assignment statements");
    registerTest(checkParserReturn, "Check if we can parse return statements");
    registerTest(checkParserExpression, "Check if we can parse expression statements");
    registerTest(checkParserIfBlock, "Check if we can parse if blocks");
    registerTest(checkParserFunctionBlock, "Check if we can parse function blocks");
    runTests();
    return 0;
}
