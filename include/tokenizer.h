#pragma once
#include "tokens.h"
#include <vector>

/*
@brief : This returns a vector of tokens, from one line of src code
*/
FORGE_API std::vector<Token>                tokenize        (const char* SRC_CODE);

/*
@brief : This returns a 2D vector of tokens, each vector corresponds to tokens from one line of a file
*/
FORGE_API std::vector<std::vector<Token>>   tokenizeFile    (const char* FILE_PATH);

/*
@brief : This prints a token
*/
FORGE_API void                              printToken      (Token* TOKEN);

/*
@brief : This prints a list of tokens
*/
FORGE_API void                              printTokens     (std::vector<Token>* TOKENS);
