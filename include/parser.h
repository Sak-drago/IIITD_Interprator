#pragma once 
#include "../defines.h"
#include "ast.h"
#include "tokens.h"
#include "program.h"
#include <vector>


FORGE_API void  produceAST(std::vector<Token>* TOKENS, Program* PROGRAM);
FORGE_API Node* parseExpression(int precedence);
FORGE_API Node* parseIdentifier(void* arg);
FORGE_API Node* parseInfixExpression(void* arg);
FORGE_API Node* parsePrefixExpression(void* arg);
FORGE_API Node* parseInteger(void* arg);
FORGE_API Node* parseBoolean(void* arg);
FORGE_API Node* parseGroupedExpress(void* arg);
FORGE_API Node* parseNormalAssignmentExpression();
FORGE_API Node* parseReturnStatement();
FORGE_API Node* parseAssignmentExpression();
FORGE_API Node* parseStatement();
