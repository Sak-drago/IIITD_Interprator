#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include <vector>
#include <iostream>

static Program* program;
static std::vector<Token>* input = NULL;
static u64 tokenIndex = 0;
static u64 allocCount = 0;
std::unordered_map<TokenType, Node*(*)(void*)> prefixParseFunctions;
std::unordered_map<TokenType, Node*(*)(void*)> infixParseFunctions;

// - - - Helper Methods - - -

typedef enum Precendence{
  EQUALS_PRECEDENCE,
  NOT_EQUALS_PRECEDENCE,
  LESSER_PRECDEENCE,
  GREATER_PRECEDENCE,
  LESSER_EQUALS_PRECEDENCE,
  GREATER_EQUALS_PRECEDENCE,
  BINARY_OPERATOR_PRECEDENCE,
  NOT_PRECEDENCE,
} Precedence;

bool match(TokenType EXPECTED_TYPE)
{
  FORGE_ASSERT_MESSAGE(input != NULL, "Cannot begin matching before recieveing input");

  if (tokenIndex < input->size() && input->at(tokenIndex).type == EXPECTED_TYPE)
  {
    tokenIndex++;
    return true;
  }
  FORGE_LOG_FATAL("Syntax error : at token index %d, expected a type of %s but recieved %s", 
                  tokenIndex, 
                  getTokenTypeString(EXPECTED_TYPE).c_str(), 
                  getTokenTypeString(input->at(tokenIndex).type).c_str());
  return false;
}

void registerPrefix(TokenType type, Node* (*parseFunction)(void*))
{
  prefixParseFunctions[type] = parseFunction;
}

void registerInfix(TokenType type, Node* (*parseFunction)(void*))
{
  infixParseFunctions[type] = parseFunction;
}

// - - - Parsing Methods - - -

Node* parseStatement()
{
  FORGE_ASSERT_MESSAGE(input != NULL, "Cannot begin parsing before recieveing input");

  Token token = input->at(tokenIndex); 

  switch (token.type)
  {
    case PLAG: 
      FORGE_LOG_DEBUG("Parsing a PLAG statement");
      return parseAssignmentExpression();

    case RETURN:
      FORGE_LOG_DEBUG("Parsing a RETURN statement");
      return parseReturnStatement();
    default:
      return parseExpressionStatement();
  }
}

// - - - NOTE: Approach followed for parsing an expression is the "PRATT PARSER" approach.

// Node* prefixParseFunction()
// {}

// Node* infixParseFunction()
// {}

Node* parseIdentifier(void* arg)
{
  Token token = input->at(tokenIndex);
  if(arg == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Variable Node");
    exit(1);
  }
  std::string variName = token.literal;
  initVariableNode((Node*) arg, variName);
  return (Node*)arg;
}

Node* parseExpressionStatement()
{
  Node* expression = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  allocCount++;
  expression = parseExpression(Precedence::EQUALS_PRECEDENCE); 

  return expression;
}

Node* parseExpression(int precedence)
{
  Token token = input->at(tokenIndex);
  Node*(*prefix)(void*) = prefixParseFunctions[token.type];
  Node* left = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  allocCount++;
  if(prefix == NULL)
  {
    FORGE_LOG_ERROR("Syntax error : expected a prefix parse function for token type %s", getTokenTypeString(input->at(tokenIndex).type).c_str());
    exit(1);
  }
  
  left = prefix((void*)left);

  return left;
}

// - - - TODO: @Asher: Ignore this for now. I will check its relevancy later.
// bool parseExpression(Node* rhs)
// {
//   Token token = input->at(tokenIndex);

//   // - - - This is interesting. The Hashmap stores <TokenType, Node* (*)()> where the Node* (*)() is a function pointer to a function that returns a Node*.
//   // - - - We create the left handside of expression. If there is no prefix parse function for the token type, we log an error.
//   Node* (*prefix)(void*) = prefixParseFunctions[token.type];
//   if(prefix == NULL)
//   {
//     FORGE_LOG_ERROR("Syntax error : expected a prefix parse function for token type %s", getTokenTypeString(token.type).c_str());
//     return false;
//   }

//   // - - - Assigning the left of the expression the prefix.
//   Node* left = prefix((void*) rhs);

//   if (token.type == IDENTIFIER)
//   {
//     std::string idenName = token.literal;
//     initVariableNode(rhs, idenName);
//     tokenIndex++;
//     return true;
//   }

//   if (token.type == NUMBER)
//   {
//     initNumberNode(rhs, std::stoi(token.literal));
//     tokenIndex++;
//     return true;
//   }

//   FORGE_LOG_ERROR("Syntax error : expected an IDENTIFIER or NUMBER but recieved %s", getTokenTypeString(token.type).c_str());
//   return false;
// }


Node* parseReturnStatement()
{
  Token token = input->at(tokenIndex);
  if (!match(RETURN))
  {
    FORGE_LOG_ERROR("Syntax error in Return statement");
    exit(1);
  }

  Node* returnNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  allocCount++;
  Node* value = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  allocCount++;
  initNumberNode(value, std::stoi(input->at(tokenIndex).literal));
  initReturnNode(returnNode, value);
  tokenIndex++;
  return returnNode;
}

Node* parseAssignmentExpression()
{
  Token token = input->at(tokenIndex+1); //- - - +1 to check the variable name

  if (!match(PLAG))
  {
    FORGE_LOG_ERROR("Syntax error in Plag statement");
    exit(1);
  }

  if (!match(IDENTIFIER))
  {
    FORGE_LOG_ERROR("Uh oh, Sakshat did not look at TODO and code me. Dies.");
    exit(1);
  }
  
  std::string variableName = input->at(tokenIndex - 1).literal;

  if (!match(ASSIGN))
  {
    FORGE_LOG_ERROR("Syntax error : expected '=' after variableName");
    exit(1);
  }
  

  FORGE_LOG_WARNING("Token literal %s", token.literal.c_str());


  // - - - TODO: @Asher: Right now, I have just assumed that we have an integer as the token and not an expression.
  Node* rhs = (Node*) linearAllocatorAllocate(&program->allocator,sizeof(Node));
  allocCount++;
  initNumberNode(rhs, std::stoi(input->at(tokenIndex).literal));
  tokenIndex++;

  Node* assigmentNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  allocCount++;
  initAssignmentNode(assigmentNode, variableName, rhs);
  return assigmentNode;
}

bool produceAST(std::vector<Token>* TOKENS, Program* PROGRAM)
{
  FORGE_ASSERT_MESSAGE(TOKENS != NULL, "TOKENS cannot be NULL to construct a program");
  FORGE_ASSERT_MESSAGE(PROGRAM != NULL, "Program cannot be NULL to construct a program");

  input = TOKENS;
  program = PROGRAM;
  tokenIndex = 0;

  // - - - Registering the prefix parse functions
  registerPrefix(IDENTIFIER, parseIdentifier);

  while (tokenIndex < input->size())
  {
    program->statements.push_back(parseStatement()); 
  }

  return true;
}
