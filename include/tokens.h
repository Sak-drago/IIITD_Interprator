#pragma once 
#include "../defines.h"
#include <string>

typedef enum TokenType
{
  ILLEGAL,
  NUMBER,
  IDENTIFIER,
  EQUALS,
  ASSIGN,
  OPEN_PARANTHESIS,
  CLOSE_PARANTHESIS,
  BINARY_OPERATOR,
  TOKEN_TYPES_COUNT,
} TokenType;

typedef struct Token 
{
  const std::string literal;
  const TokenType   type;

  // - - - ovverride the equality operator to see if two tokens are equals
  bool              operator==(const Token& OTHER) const 
  {
    return literal == OTHER.literal && type == OTHER.type;
  }
} Token;
