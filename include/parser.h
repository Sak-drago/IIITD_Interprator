#pragma once 
#include "../defines.h"
#include "ast.h"
#include "tokens.h"
#include <vector>

typedef struct Program
{
  std::vector<Node> statements;
} Program;

FORGE_API bool produceAST(std::vector<Token>* TOKENS, Program* PROGRAM);
FORGE_API Node parseAssignmentExpression();
FORGE_API Node parseStatement();
