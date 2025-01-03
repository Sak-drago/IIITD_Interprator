#include "../include/ast.h"
#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include <vector>
#include <iostream>

static Program* program;
static std::vector<Token>* input = NULL;
static u64 tokenIndex = 0;
std::unordered_map<TokenType, Node*(*)(void*)> prefixParseFunctions;
std::unordered_map<TokenType, Node*(*)(void*)> infixParseFunctions;
// - - - Helper Methods - - -

// - - - Precedence order
// - - - TODO: @Asher: Just make the precedence equal. Right now, ">" > "<".
typedef enum Precendence{
  LOWEST,
  EQUALS_PRECEDENCE,
  NOT_EQUALS_PRECEDENCE,
  LESSER_PRECDEENCE,
  GREATER_PRECEDENCE,
  LESSER_EQUALS_PRECEDENCE,
  GREATER_EQUALS_PRECEDENCE,
  BINARY_OPERATOR_PRECEDENCE,
  NOT_PRECEDENCE,
  PRECEDENCE_COUNT, // - - - To keep count of items in order
} Precedence;

// - - - Precedence Mapping
std::unordered_map<TokenType, Precedence> precedencesMapping =
{
  {EQUALS, EQUALS_PRECEDENCE},
  {NOT_EQUALS, NOT_EQUALS_PRECEDENCE},
  {LESSER, LESSER_PRECDEENCE},
  {GREATER, GREATER_PRECEDENCE},
  {LESSER_EQUAL, LESSER_EQUALS_PRECEDENCE},
  {GREATER_EQUAL, GREATER_EQUALS_PRECEDENCE},
  {BINARY_OPERATOR, BINARY_OPERATOR_PRECEDENCE},
  {NOT, NOT_PRECEDENCE}
};

// - - - Checks what Precedence does the next token have
int peekPrecedence()
{
  if (input->size() <= tokenIndex)
  {
    return LOWEST;
  }

  Token token = input->at(tokenIndex);

  if (precedencesMapping.find(token.type) != precedencesMapping.end())
  {
    return precedencesMapping[token.type];
  }

  return LOWEST;
}


// - - - Binary Operator setter
// - - - TODO: Asher: Make it pretty
BinaryOperator getBinaryOperator(Token token)
{
  if (token.type == BINARY_OPERATOR)
  {
    if (token.literal == "+") return BINARY_OPERATOR_ADDITION;
    if (token.literal == "-") return BINARY_OPERATOR_SUBTRACTION;
    if (token.literal == "*") return BINARY_OPERATOR_MULTIPLICATION;
    if (token.literal == "/") return BINARY_OPERATOR_DIVISION;
    if (token.literal == "%") return BINARY_OPERATOR_REMAINDER;
    if (token.literal == "^") return BINARY_OPERATOR_POWER;
    FORGE_LOG_ERROR("How did you even get here Asher?");
    exit(1);
  }
  FORGE_LOG_ERROR("How did you even get here Asher?");
  exit(1);
}
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

Node* parsePrefixExpression(void* arg) 
{
  Token token = input->at(tokenIndex);
  if(arg == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Prefix Node");
    exit(1);
  }
  std::string operatorType = token.literal;
  tokenIndex++;

  // - - - TODO: @Asher: Only supports "!" operator for now. Add support for "-" operator.
  Node* right = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  right = parseExpression(Precedence::NOT_PRECEDENCE);
  initPrefixNode((Node*) arg, operatorType.c_str(), right);
  return (Node*)arg;
}

Node* parseInfixExpression(void* arg)
{
  Token token = input->at(tokenIndex);
  if(arg == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Binary Operator Node");
    exit(1);
  }
  BinaryOperator opcode = getBinaryOperator(token);
  int precedence = peekPrecedence();
  tokenIndex++;
  Node* right = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  right = parseExpression(precedence);
  initBinaryOpNode((Node*) arg, (Node*) arg, right, opcode);
  return (Node*)arg;
}

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
  tokenIndex++;
  return (Node*)arg;
}

Node* parseInteger(void* arg)
{
  Token token = input->at(tokenIndex);
  if(arg == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Number Node");
    exit(1);
  }
  initNumberNode((Node*) arg, std::stoi(token.literal));
  tokenIndex++;
  return (Node*)arg;
}

Node* parseExpressionStatement()
{
  Node* expression = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  expression = parseExpression(Precedence::LOWEST); 

  return expression;
}

Node* parseExpression(int precedence)
{
  Token token = input->at(tokenIndex);
  Node*(*prefix)(void*) = prefixParseFunctions[token.type];
  Node* left = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  if(prefix == NULL)
  {
    FORGE_LOG_ERROR("Syntax error : expected a prefix parse function for token type %s", getTokenTypeString(input->at(tokenIndex).type).c_str());
  }
  else left = prefix((void*)left);

  while (precedence < peekPrecedence())
  {
    Node*(*infix)(void*) = infixParseFunctions[token.type];
    if(infix == NULL)
    {
      FORGE_LOG_DEBUG("No Infix parser found for %s", getTokenTypeString(input->at(tokenIndex).type).c_str());
      return left;
    }
    left = infix((void*)left);
  }

  return left;
}

Node* parseReturnStatement()
{
  Token token = input->at(tokenIndex);
  if (!match(RETURN))
  {
    FORGE_LOG_ERROR("Syntax error in Return statement");
    exit(1);
  }

  Node* returnNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  Node* value = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
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
    FORGE_LOG_ERROR("No variable to assign to.");
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
  initNumberNode(rhs, std::stoi(input->at(tokenIndex).literal));
  tokenIndex++;

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

  // - - - Registering the parse functions
  registerPrefix(NOT, parsePrefixExpression);
  registerPrefix(IDENTIFIER, parseIdentifier);
  registerPrefix(NUMBER, parseInteger);
  registerInfix(EQUALS, parseInfixExpression);
  registerInfix(NOT_EQUALS, parseInfixExpression);
  registerInfix(LESSER, parseInfixExpression);
  registerInfix(GREATER, parseInfixExpression);
  registerInfix(LESSER_EQUAL, parseInfixExpression);
  registerInfix(GREATER_EQUAL, parseInfixExpression);
  registerInfix(BINARY_OPERATOR, parseInfixExpression);

  while (tokenIndex < input->size())
  {
    program->statements.push_back(parseStatement()); 
  }

  return true;
}
