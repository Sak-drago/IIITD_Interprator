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

FORGE_API void  produceAST(std::vector<Token>* TOKENS, Program* PROGRAM);
FORGE_API Node* parseExpression(int precedence);
FORGE_API Node* parseIdentifier(void* arg);
FORGE_API Node* parseInfixExpression(void* arg);
FORGE_API Node* parsePrefixExpression(void* arg);
FORGE_API Node* parseInteger(void* arg);
FORGE_API Node* parseBoolean(void* arg);
FORGE_API Node* parseGroupedExpress(void* arg);
FORGE_API Node* parseReturnStatement();
FORGE_API Node* parseAssignmentExpression();
FORGE_API Node* parseStatement();
