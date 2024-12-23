#include "tokenizerTests.h"
#include "../include/tokenizer.h"
#include "../library/include/filesystem.h"
#include <vector>

u8 checkTokenizeCustomString()
{
  const char* customString = "+=-()";
  std::vector<Token> tokens = tokenize(customString);

  // - - - Define the expected tokens
  std::vector<Token> expectedTokens = 
  {
    Token{"+", BINARY_OPERATOR},
    Token{"=", EQUALS},
    Token{"-", BINARY_OPERATOR},
    Token{"(", OPEN_PARANTHESIS},
    Token{")", CLOSE_PARANTHESIS},
  };

  // - - - Check if the tokenization matches the expected tokens
  expectShouldBe(expectedTokens.size(), tokens.size());

  // - - - Iterate through tokens and compare with expected
  for (u64 i = 0; i < tokens.size(); i++) 
  {
    expectShouldBe(expectedTokens[i].type, tokens[i].type);
    expectStringToBe(expectedTokens[i].literal.c_str(), tokens[i].literal.c_str());
  }

  FORGE_LOG_INFO("Generated %d tokens out of %s", tokens.size(), customString);
  printTokens(&tokens);

  return true;
}

u8 checkTokenizeFile()
{
  // - - - Create and open a file for writing
  File writeFile;
  expectToBeTrue(openFile("testFile.txt", FILE_MODE_WRITE, false, &writeFile));

  // - - - Write multiple test lines to the file
  expectToBeTrue(writeFileLine(&writeFile, "(+)=(-)"));
  expectToBeTrue(writeFileLine(&writeFile, "((=))+"));

  // - - - Close the file after writing
  closeFile(&writeFile);

  // - - - Tokenize the file
  std::vector<std::vector<Token>> tokenizedLines = tokenizeFile("testFile.txt");

  // - - - Expected tokens for each line
  std::vector<std::vector<Token>> expectedTokenizedLines = 
  {
    { // - - - Line 1: "(+)=(-)"
      Token{"(", OPEN_PARANTHESIS},
      Token{"+", BINARY_OPERATOR},
      Token{")", CLOSE_PARANTHESIS},
      Token{"=", EQUALS},
      Token{"(", OPEN_PARANTHESIS},
      Token{"-", BINARY_OPERATOR},
      Token{")", CLOSE_PARANTHESIS},
    },
    { // - - - Line 2: "((=))+"
      Token{"(", OPEN_PARANTHESIS},
      Token{"(", OPEN_PARANTHESIS},
      Token{"=", EQUALS},
      Token{")", CLOSE_PARANTHESIS},
      Token{")", CLOSE_PARANTHESIS},
      Token{"+", BINARY_OPERATOR},
    }
  };

  // - - - Validate the number of lines tokenized
  expectShouldBe(expectedTokenizedLines.size(), tokenizedLines.size());

  // - - - Validate tokens for each line
  for (u64 lineIndex = 0; lineIndex < tokenizedLines.size(); ++lineIndex) 
  {
    std::vector<Token>& tokens            = tokenizedLines[lineIndex];
    std::vector<Token>& expectedTokens    = expectedTokenizedLines[lineIndex];

    // - - - Validate the number of tokens on the current line
    expectShouldBe(expectedTokens.size(), tokens.size());

    // - - - Validate each token
    for (u64 i = 0; i < tokens.size(); ++i) 
    {
      expectShouldBe(expectedTokens[i].type, tokens[i].type);
      expectStringToBe(expectedTokens[i].literal.c_str(), tokens[i].literal.c_str());
    }
  
    FORGE_LOG_INFO("Generated %d tokens", tokens.size());
    printTokens(&tokens);
  }

  return true;
}

int main () 
{
  registerTest(checkTokenizeCustomString, "Check if we can tokenize a custom string");  
  registerTest(checkTokenizeFile, "Check if we can tokenize a file");
  runTests();
  return 0;
}
