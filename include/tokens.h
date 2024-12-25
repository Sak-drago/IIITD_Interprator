#pragma once 
#include "../defines.h"
#include <string>
#include <unordered_map>

typedef enum TokenType
{
  ILLEGAL,
  NUMBER,
  IDENTIFIER,
  EQUALS,
  NOT_EQUALS,
  NOT,
  ASSIGN,
  OPEN_PARANTHESIS,
  CLOSE_PARANTHESIS,
  BINARY_OPERATOR,
  GREATER,
  LESSER,
  GREATER_EQUAL,
  LESSER_EQUAL,
  PLAG,
  DAC,
  TRUE,
  FALSE,
  IF,
  ELIF,
  ELSE,
  FOR,
  WHILE,
  TOKEN_TYPES_COUNT,
} TokenType;


extern std::unordered_map<std::string, TokenType> keywords;

TokenType lookUpKeywords(std::string &ident);

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
