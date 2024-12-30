#pragma once 
#include "../defines.h"
#include "ast.h"
#include "tokens.h"
#include "../library/include/linearAlloc.h"
#include <vector>

typedef struct Program
{
  std::vector<Node*> statements;
  LinearAllocator allocator;
} Program;

FORGE_API bool  produceAST(std::vector<Token>* TOKENS, Program* PROGRAM);
FORGE_API Node* parseAssignmentExpression();
FORGE_API Node* parseStatement();
