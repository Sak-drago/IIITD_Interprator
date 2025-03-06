#include "include/ast.h"
#include "include/evaluator.h"
#include "include/program.h"
#include "include/tokenizer.h"
#include "include/parser.h"
#include "library/include/filesystem.h"
#include "library/include/linearAlloc.h"
#include "library/include/logger.h"

int main (int ARGUMENT_COUNT, char* ARGUMENT_VECTOR[]) 
{
  // - - - mistake or need help
  if (ARGUMENT_COUNT == 1 || (ARGUMENT_COUNT == 2 && strcmp(ARGUMENT_VECTOR[1], "--h") == 0))
  {
    FORGE_LOG_INFO("Usage : -");
    FORGE_LOG_DEBUG("program --f <FILE_PATH> : Run the file at <FILE_PATH>");
    FORGE_LOG_DEBUG("program --s <SRC_CODE>  : Tokenize the <SRC_CODE>");
    FORGE_LOG_DEBUG("program --p <SRC_CODE>  : Tokenize and Parse the <SRC_CODE>");
    FORGE_LOG_DEBUG("program --r <SRC_CODE>  : Run the <SRC_CODE>");
    FORGE_LOG_DEBUG("program --h             : You are already here");
    FORGE_LOG_DEBUG("program <INPUT>         : Let the program decide whether your input is a file or SRC_CODE");
    return 0;
  }

  if (ARGUMENT_COUNT == 3)
  {
    // - - - run a file
    if (strcmp(ARGUMENT_VECTOR[1], "--f") == 0)
    {
      FORGE_LOG_INFO("Beginning Tokenization of %s-", ARGUMENT_VECTOR[2]);
      std::vector<std::vector<Token>> tokens = tokenizeFile(ARGUMENT_VECTOR[2]);
      size_t size = 0;
      for (std::vector<Token>& tok : tokens)
      {
        size += tok.size();
      }
      for (std::vector<Token>& tok : tokens)
      {
        printTokens(&tok);
      }
      FORGE_LOG_INFO("%d Tokens generated out of file at : %s\n", size, ARGUMENT_VECTOR[2]);

      FORGE_LOG_INFO("Beginning Parsing of %s-", ARGUMENT_VECTOR[2]);
      // - - - 1 MB assigned for now
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.allocator);
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.global.memory);
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.stack.memory);
      for (auto i : tokens) produceAST(&i, &runtime);
      for (Node* node : runtime.statements)
      {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
      }
      FORGE_LOG_INFO("AST ^\n");

      FORGE_LOG_INFO("Beginning evaluation");
      run();

      destroyLinearAllocator(&runtime.allocator);
      destroyLinearAllocator(&runtime.global.memory);
      destroyLinearAllocator(&runtime.stack.memory);
      return 0;
      return 0;
    }

    // - - - tokenize a line 
    if (strcmp(ARGUMENT_VECTOR[1], "--s") == 0)
    {
      std::vector<Token> tokens = tokenize(ARGUMENT_VECTOR[2]);
      FORGE_LOG_INFO("%d Tokens generated out of source code : %s", tokens.size(), ARGUMENT_VECTOR[2]);
      printTokens(&tokens);
      return 0;
    }

    // - - - tokenize and parse a line
    if (strcmp(ARGUMENT_VECTOR[1], "--p") == 0)
    {
      std::vector<Token> tokens = tokenize(ARGUMENT_VECTOR[2]);
      FORGE_LOG_INFO("%d Tokens generated out of source code : %s", tokens.size(), ARGUMENT_VECTOR[2]);
      printTokens(&tokens);

      Program program;
      // - - - 1 MB assigned for now
      createLinearAllocator(1024 * 1024, 0, NULL, &program.allocator);
      produceAST(&tokens, &program);

      for (Node* node : program.statements)
      {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
      }

      destroyLinearAllocator(&program.allocator);
      return 0;
    }
    
    // - - - tokenize and parse and run a line
    if (strcmp(ARGUMENT_VECTOR[1], "--r") == 0)
    {
      std::vector<Token> tokens = tokenize(ARGUMENT_VECTOR[2]);
      FORGE_LOG_INFO("%d Tokens generated out of source code : %s", tokens.size(), ARGUMENT_VECTOR[2]);
      printTokens(&tokens);

      // - - - 1 MB assigned for now
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.allocator);
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.global.memory);
      createLinearAllocator(1024 * 1024, 0, NULL, &runtime.stack.memory);
      produceAST(&tokens, &runtime);

      for (Node* node : runtime.statements)
      {
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
      }

      FORGE_LOG_INFO("\nBeginning Evaluation");
      for (Node* node : runtime.statements)
      {
        Data data = evaluate(node);
        FORGE_LOG_DEBUG(getDataStr(&data).c_str());
      }

      FORGE_LOG_INFO("Global Environment");
      auto it = runtime.global.pointers.begin();
      while (it != runtime.global.pointers.end()) 
      {
        FORGE_LOG_DEBUG("Variable Name : %s\tAddress:%p", it->first.c_str(), it->second.value);
        ++it;
      }
      
      FORGE_LOG_INFO("Local Environment");
      it = runtime.stack.pointers.begin();
      while (it != runtime.stack.pointers.end()) 
      {
        FORGE_LOG_DEBUG("Variable Name : %s\tAddress:%p", it->first.c_str(), it->second.value);
        ++it;
      }

      destroyLinearAllocator(&runtime.allocator);
      destroyLinearAllocator(&runtime.global.memory);
      destroyLinearAllocator(&runtime.stack.memory);
      return 0;
    }
  }

  // - - - decide on your own whether we are tokenizing file or line 
  if (ARGUMENT_COUNT == 2)
  {
    if (fileExists(ARGUMENT_VECTOR[1]))
    {
      std::vector<std::vector<Token>> tokens = tokenizeFile(ARGUMENT_VECTOR[1]);
      size_t size = 0;
      for (std::vector<Token>& tok : tokens)
      {
        size += tok.size();
      }
      FORGE_LOG_INFO("%d Tokens generated out of file at : %s", size, ARGUMENT_VECTOR[1]);

      for (std::vector<Token>& tok : tokens)
      {
        printTokens(&tok);
      }
      return 0;
    }
    else 
    {
      std::vector<Token> tokens = tokenize(ARGUMENT_VECTOR[1]);
      FORGE_LOG_INFO("%d Tokens generated out of source code : %s", tokens.size(), ARGUMENT_VECTOR[1]);
      printTokens(&tokens);
      return 0;
    }
    return 0;
  }

  FORGE_LOG_ERROR("Invalid usage");
}
