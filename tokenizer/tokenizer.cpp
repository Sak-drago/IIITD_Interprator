#include "../include/tokenizer.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../library/include/filesystem.h"
#include <vector>

// - - - Helper Functions - - -


// - - - helper function to generate a token
Token makeToken(const char* TOKEN, int START, int END, TokenType TYPE)
{
  FORGE_ASSERT_MESSAGE(TOKEN != NULL,   "Cannot make a NULL token");
  FORGE_ASSERT_MESSAGE(START >= 0,      "First character of the token must start ");

  // - - - construct the token
  std::string literal(TOKEN + START, END - START);
  return Token {literal, TYPE};
}

// - - - helper function to get string representation of token Type
const std::string tokenNames[TOKEN_TYPES_COUNT] = 
{
  "ILLEGAL",
  "NUMBER",
  "IDENTIFIER",
  "EQUALS",
  "ASSIGN",
  "OPEN_PARANTHESIS",
  "CLOSE_PARANTHESIS",
  "BINARY_OPERATOR",
};

const std::string getTokenTypeString(TokenType TYPE)
{
  FORGE_ASSERT_MESSAGE(TYPE != TOKEN_TYPES_COUNT, "TOKEN_TYPES_COUNT is not a valid token type, it exists only to get a count of how many kinds of tokens we have.");
  return tokenNames[TYPE];
}

// - - - Interface Functions - - -


// - - - function to get a vector of tokens out of source code
std::vector<Token> tokenize(const char* SRC_CODE)
{
  FORGE_ASSERT_MESSAGE(SRC_CODE != NULL, "SRC_CODE cannot be NULL to tokenize");

  // - - - tokens will be what we return
  std::vector<Token> tokens;
  // - - - a copy because we dont want to modify the original string
  const char*        src                   = SRC_CODE;
  int                start                 = 0;
  int                current               = 0;

  while (src[current] != '\0' && src[current] != '\n')
  {
    char currentChar = src[current];
    switch (currentChar)
    {
      // - - - OPEN_PARANTHESIS
      case '(':
        tokens.push_back(makeToken(
              SRC_CODE,     
              current, 
              current + 1, 
              OPEN_PARANTHESIS));
        current++;
        break;

      // - - - CLOSE_PARANTHESIS
      case ')':
        tokens.push_back(makeToken(
              SRC_CODE,     
              current, 
              current + 1, 
              CLOSE_PARANTHESIS));
        current++;
        break;

      // - - - EQUALS  
      case '=':
        tokens.push_back(makeToken(
              SRC_CODE,     
              current, 
              current + 1, 
              EQUALS));
        current++;
        break;

      // - - - BINARY_OPERATOR
      case '+':
      case '-':
      case '*':
      case '/':
      case '^':
      case '%':
        tokens.push_back(makeToken(
              SRC_CODE,     
              current, 
              current + 1, 
              BINARY_OPERATOR));
        current++;
        break;

      // - - - handle multi character token later
      default : 
        break;
    }
  }

  return tokens;
}

// - - - function to tokenize a whole file
std::vector<std::vector<Token>> tokenizeFile(const char* FILE_PATH)
{
  // - - - check if conditions are favorable
  FORGE_ASSERT_MESSAGE(FILE_PATH != NULL,      "FILE_PATH cannot be null to tokenize");
  FORGE_ASSERT_MESSAGE(fileExists(FILE_PATH),  "File must exist to tokenize it");

  // - - - try to open file
  File sourceCodeFile;
  bool openSucces       = openFile(FILE_PATH, FILE_MODE_READ, false, &sourceCodeFile);

  FORGE_ASSERT_MESSAGE(openSucces,              "Cannot fail to open file in order to tokenize");


  // - - - generate tokens from each file line
  char* line;
  std::vector<std::vector<Token>> fileTokens;

  while (readFileLine(&sourceCodeFile, &line))
  {
    fileTokens.push_back(tokenize(line));
    free(line);
  }

  // - - - close the file and return;
  closeFile(&sourceCodeFile);
  return fileTokens;
}

// - - - simple function to print one token
void printToken(Token* TOKEN)
{
  FORGE_ASSERT_MESSAGE(TOKEN != NULL, "Cannot print a NULL token");

  std::string tokenType     = getTokenTypeString(TOKEN->type);
  std::string tokenLiteral  = TOKEN->literal;

  FORGE_LOG_DEBUG("Token Type : \t%-15s \t\tToken Message : %-20s", tokenType.c_str(), tokenLiteral.c_str());
}

// - - - print many tokens
void printTokens(std::vector<Token>* TOKENS)
{
  FORGE_ASSERT_MESSAGE(TOKENS != NULL, "Cannot print a NULL vector of tokens");

  for (Token token : *TOKENS)
  {
    printToken(&token);
  }
}
