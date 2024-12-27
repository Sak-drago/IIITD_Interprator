#include "../include/parser.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"


// - - - Parser constructor
Parser::Parser(const std::vector<Token>& TOKENS) : tokens(TOKENS), index(0) {}



// - - - Get working token - - -

// - - - peek current token
Token Parser::peek() const 
{
  if (index < tokens.size()) 
  {
    return tokens[index];
  }
  FORGE_LOG_ERROR("Parser has no more tokens to peek");
  return Token {":(", ILLEGAL};
}

// - - - goto the next token
Token Parser::advance() 
{
  if (index < tokens.size()) 
  {
    return tokens[index++];
  }
  FORGE_LOG_ERROR("Parser has no more tokens to advance");
  return Token {":(", ILLEGAL};
}


// - - - use a token - - -

// - - - see if token matches what we want
bool Parser::match(TokenType TYPE) 
{
  if (peek().type == TYPE) 
  {
    advance();
    return true;
  }
  return false;
}

// - - - use the token and destroy it
Token Parser::consume(TokenType TYPE, const std::string& EXCEPTION) 
{
  if (match(TYPE)) 
  {
    return tokens[index - 1];
  }
  FORGE_LOG_ERROR(EXCEPTION.c_str());
  return Token {":(", ILLEGAL};
}


// - - - Parse Stuff - - - 

// - - - NOTE: A statement != expression. A program is made of statements not expressions
Statement* Parser::parseStatement()
{
  // - - - WARNING: Incomplete
  return parseExpression();
}

// - - - parse an expression
Expression* Parser::parseExpression() 
{
  Expression* left = parsePrimary();

  // - - - Check for binary operator
  if (match(BINARY_OPERATOR)) 
  {
    return parseBinary(left);
  }

  return left;
}

// - - - parse the first visible expression
Expression* Parser::parsePrimary() 
{
  Token token = peek();

  switch (token.type) 
  {
    case NUMBER:
      return parseImmediate();

    case IDENTIFIER:
      return parseVariable();

    default:
      FORGE_LOG_ERROR("%s is currently unsupported by the parser", getTokenTypeString(token.type).c_str());
      exit(1);
  }
}

// - - - parse a number expression
ImmediateExpr* Parser::parseImmediate() 
{
  Token token = consume(NUMBER, "Expected a number");
  u64   value = std::stoull(token.literal);
  return new ImmediateExpr(value);
}

// - - - parse a variable expression
VariableExpr* Parser::parseVariable() 
{
  Token token = consume(IDENTIFIER, "Expected an identifier");

  // - - - WARNING: acche se address nahi dala hai yet and assumed size is 64
  return new VariableExpr(token.literal, 0, 64);
}

// - - - parse an expression with some sort of operation
BinaryExpr* Parser::parseBinary(Expression* LEFT) 
{
  Token operatorToken   = consume(BINARY_OPERATOR, "Expected a binary operator");
  Expression *right     = parseExpression();

  return new BinaryExpr(LEFT, right, ADDITION);
}


// - - - The useful function to parse everything and construct a program out of it
Program Parser::parse() 
{
  Program program;

  while (index < tokens.size()) 
  {
    Statement* expression = parseStatement();
    program.statements.push_back(*expression);
  }

  return program;
}
