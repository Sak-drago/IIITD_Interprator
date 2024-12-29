#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include <vector>

static Program* program;
static std::vector<Token>* input = NULL;
static u64 tokenIndex = 0;


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


Node parseStatement()
{
  FORGE_ASSERT_MESSAGE(input != NULL, "Cannot begin parsing before recieveing input");

  Token token = input->at(tokenIndex); 

  switch (token.type)
  {
    case PLAG: 
      return parseAssignmentExpression();

    default:
      FORGE_LOG_FATAL("Not yet handled parsing of %s", getTokenTypeString(token.type).c_str());
      exit(1);
  }
}

Node parseAssignmentExpression()
{
  Token token = input->at(tokenIndex);

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

  if (!match(EQUALS))
  {
    FORGE_LOG_ERROR("Syntax error : expected '=' after variableName");
    exit(1);
  }

  // - - - TODO: make this its own function and be ready to assign any node to a variable
  if (!match(NUMBER))
  {
    FORGE_LOG_ERROR("Uh oh, Sakshat did not look at TODO and code me. Dies.");
    exit(1);
  }
    
  Node rhs;
  initNumberNode(&rhs, std::stoi(token.literal));

  program->statements.push_back(rhs);

  Node assigmentNode;
  initAssignmentNode(&assigmentNode, variableName, &rhs);

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
