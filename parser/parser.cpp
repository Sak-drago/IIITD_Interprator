#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include <vector>

static Program* program;
static std::vector<Token>* input = NULL;
static u64 tokenIndex = 0;
std::unordered_map<TokenType, Node*(*)(void*)> prefixParseFunctions;
std::unordered_map<TokenType, Node*(*)(void*)> infixParseFunctions;

// - - - Helper Methods - - -

typedef enum Precendence{
  EQUALS,
  NOT_EQUALS,
  LESSER,
  GREATER,
  LESSER_EQUALS,
  GREATER_EQUALS,
  BINARY_OPERATOR,
  NOT,
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
      return parseAssignmentExpression();

    case RETURN:
      return parseReturnStatement();
    default:
      FORGE_LOG_FATAL("Not yet handled parsing of %s", getTokenTypeString(token.type).c_str());
      exit(1);
  }
}

// - - - NOTE: Approach followed for parsing an expression is the "PRATT PARSER" approach.

Node* prefixParseFunction()
{}

Node* infixParseFunction()
{}

Node* parseIdentifier(void* arg)
{
  Token token = input->at(tokenIndex);
  std::string idenName = token.literal;
  initVariableNode((Node*) arg, idenName);
  return idenNode;
}

bool parseExpression(Node* rhs)
{
  Token token = input->at(tokenIndex);

  // - - - This is interesting. The Hashmap stores <TokenType, Node* (*)()> where the Node* (*)() is a function pointer to a function that returns a Node*.
  // - - - We create the left handside of expression. If there is no prefix parse function for the token type, we log an error.
  Node* (*prefix)(void*) = prefixParseFunctions[token.type];
  if(prefix == NULL)
  {
    FORGE_LOG_ERROR("Syntax error : expected a prefix parse function for token type %s", getTokenTypeString(token.type).c_str());
    return false;
  }

  // - - - Assigning the left of the expression the prefix.
  Node* left = prefix((void*) rhs);

  if (token.type == IDENTIFIER)
  {
    std::string idenName = token.literal;
    initVariableNode(rhs, idenName);
    tokenIndex++;
    return true;
  }

  if (token.type == NUMBER)
  {
    initNumberNode(rhs, std::stoi(token.literal));
    tokenIndex++;
    return true;
  }

  FORGE_LOG_ERROR("Syntax error : expected an IDENTIFIER or NUMBER but recieved %s", getTokenTypeString(token.type).c_str());
  return false;
}


Node* parseReturnStatement()
{
  if (!match(RETURN))
  {
    FORGE_LOG_ERROR("Syntax error in Return statement");
    exit(1);
  }

  Node* returnNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  if (!parseExpression(returnNode))
  {
    FORGE_LOG_ERROR("Syntax error in parsing expression");
    exit(1);
  }

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
  

  // - - - TODO: make this its own function and be ready to assign any node to a variable
  // - - - NOTE: Approach to follow that I am thinking is to have another seperate function known as parseExpresison. If the Expression is an IDENTIFIER then it is fine, else we paraseNumber.

    // - - - TODO: @Sakshat
  FORGE_LOG_WARNING("Token literal %s", token.literal.c_str());

  Node* rhs = (Node*) linearAllocatorAllocate(&program->allocator,sizeof(Node));
  
  // - - - If parsing fails
  if(!parseExpression(rhs))
  {
    FORGE_LOG_ERROR("Syntax error in parsing expression");
    exit(1);
  }

  // - - - WARNING: at the moment I am too sleepy to make a tree structure. If you want me I would
  Node* assigmentNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
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

  while (tokenIndex < input->size())
  {
    program->statements.push_back(parseStatement()); 
  }


  return true;
}
