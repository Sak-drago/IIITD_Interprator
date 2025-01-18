#include "../include/ast.h"
#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include "../include/tokens.h"
#include <vector>

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
  LESSER_EQUALS_PRECEDENCE,
  BINARY_OPERATOR_PRECEDENCE,
  SUM,
  PRODUCT,
  NOT_PRECEDENCE,
  PRECEDENCE_COUNT, // - - - To keep count of items in order
} Precedence;

// - - - Precedence Mapping
std::unordered_map<TokenType, Precedence> precedencesMapping =
{
  {EQUALS, EQUALS_PRECEDENCE},
  {NOT_EQUALS, NOT_EQUALS_PRECEDENCE},
  {LESSER, LESSER_PRECDEENCE},
  {GREATER, LESSER_PRECDEENCE},
  {LESSER_EQUAL, LESSER_EQUALS_PRECEDENCE},
  {GREATER_EQUAL, LESSER_EQUALS_PRECEDENCE},
  {BINARY_OPERATOR, BINARY_OPERATOR_PRECEDENCE},
  {ADD, SUM},
  {SUB, SUM},
  {MUL, PRODUCT},
  {DIV, PRODUCT},
  {NOT, NOT_PRECEDENCE}
};

// - - - Checks what Precedence does the next token have
int curPrecedence()
{
  if (input->size() <= tokenIndex)
  {
    return LOWEST;
  }

  Token token = input->at(tokenIndex);

  if (precedencesMapping.find(token.type) != precedencesMapping.end())
  {
    if(token.type == BINARY_OPERATOR)
    {
      switch (token.literal[0])
      {
      case '+':
      case '-':
        return SUM;
        break;
      case '*':
      case '/':
        return PRODUCT;
        break;
      default:
        return precedencesMapping[token.type];
        break;
      }
    }
    return precedencesMapping[token.type];
  }

  return LOWEST;
}

int peekPrecedence()
{
  if (input->size() <= tokenIndex+1)
  {
    return LOWEST;
  }

  Token token = input->at(tokenIndex+1);

  if (precedencesMapping.find(token.type) != precedencesMapping.end())
  {
    return precedencesMapping[token.type];
  }

  return LOWEST;
}


// - - - Binary Operator setters - - -

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
  else
  {
    if(token.literal == "<") return COMPARISON_OPERATOR_GREATER;
    if(token.literal == ">") return COMPARISON_OPERATOR_LESSER;
    if(token.literal == "==") return COMPARISON_OPERATOR_EQUAL;
    if(token.literal == "<=") return COMPARISON_OPERATOR_GREATER_EQUAL;
    if(token.literal == "=>") return COMPARISON_OPERATOR_LESSER_EQUAL;
    FORGE_LOG_ERROR("How did you even get here Asher but for Comparison?");
  }
  FORGE_LOG_ERROR("How did you even get here Asher? Part 2, %s, %i", token.literal.c_str(), tokenIndex);
  exit(1);
}


// - - - Match the token type and move forward
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

void registerPrefix(TokenType TYPE, Node* (*PARSE_FUNCTIONS)(void*))
{
  prefixParseFunctions[TYPE] = PARSE_FUNCTIONS;
}

void registerInfix(TokenType type, Node* (*parseFunction)(void*))
{
  infixParseFunctions[type] = parseFunction;
}


// - - - Parsing Methods - - -

// - - - Statement
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

// - - - Block Statement

Block* parseBlockStatement()
{
  Block* block = (Block*) linearAllocatorAllocate(&program->allocator, sizeof(Block));
  block->statements = std::vector<Node*>();

  while (tokenIndex < input->size() && input->at(tokenIndex).type != CLOSE_BRACE)
  {
    FORGE_LOG_DEBUG("Parsing a block statement");
    block->statements.push_back(parseStatement());
  }
  return block;
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
  Node* left = (Node*)linearAllocatorAllocate(&program->allocator, sizeof(Node));
  Token token = input->at(tokenIndex);
  if(arg == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Binary Operator Node");
    exit(1);
  }

  BinaryOperator opcode = getBinaryOperator(token);
  int precedence        = curPrecedence();
  tokenIndex++;

  Node* right = nullptr;
  right = parseExpression(precedence);
  if(match(CLOSE_PARANTHESIS)){}
  initBinaryOpNode(left, (Node*)arg , right, opcode);
  tokenIndex++;
  return left;
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
  return (Node*)arg;
}

Node* parseExpressionStatement()
{
  Node* expression = nullptr;
  expression = parseExpression(Precedence::LOWEST);
  return expression;
}

Node* parseBoolean(void* arg)
{
  Token token = input->at(tokenIndex);
  FORGE_LOG_TRACE("Parsing a boolean value");
  if(token.type == TRUE)
  { 
    initBoolNode((Node*)arg, true);
    return (Node*)arg;
  }
  else if(token.type == FALSE)
  {
    initBoolNode((Node*)arg, false);
    return (Node*)arg;
  }
  else
  {
    FORGE_LOG_ERROR("Syntax error : expected a boolean value");
    exit(1);
  }
}

Node* parseGroupedExpress(void* arg)
{
  tokenIndex++;
  Node* expression = parseExpression(Precedence::LOWEST);
  if (!match(CLOSE_PARANTHESIS))
  {
    FORGE_LOG_ERROR("Syntax error : expected a closing paranthesis");
    exit(1);
  }
  return expression;
}

// - - - Only supports "IF" with/without "ELSE" statement for now
// - - - TODO: @Asher: Add ELIF support
Node* parseIfExpression(void* arg)
{
  Token token = input->at(tokenIndex);
  if (!match(IF))
  {
    FORGE_LOG_ERROR("Syntax error in If statement");
    exit(1);
  }

  if(!match(OPEN_PARANTHESIS))
  {
    FORGE_LOG_ERROR("Syntax error in If statement");
    exit(1);
  }


  Node* condition = nullptr;
  condition = parseExpression(Precedence::LOWEST);
  tokenIndex++;
  
  // - - - Skipping over the close paranthesis
  if(match(CLOSE_PARANTHESIS)){}


  if(!match(OPEN_BRACE))
  {
    FORGE_LOG_ERROR("Syntax error in If statement");
    exit(1);
  }

  Block* consequence = nullptr;
  consequence = parseBlockStatement();
  tokenIndex++;
  Block* alternative = nullptr;
  if (tokenIndex < input->size() && match(ELSE))
  {
    alternative = parseBlockStatement();
  }

  initIfNode((Node*)arg, condition, consequence, alternative);
  return (Node*)arg;
}

Node* parseExpression(int precedence)
{
  Token token = input->at(tokenIndex);
  auto it = prefixParseFunctions.find(token.type);
  Node* left = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  if(it == prefixParseFunctions.end())
  {
    FORGE_LOG_ERROR("Syntax error : expected a prefix parse function for token type %s", getTokenTypeString(input->at(tokenIndex).type).c_str());
  }
  else
  {
    Node*(*prefix)(void*) = it->second;
    left = prefix((void*)left);
  }

  while (precedence < peekPrecedence())
  {
    tokenIndex++;
    auto it = infixParseFunctions.find(input->at(tokenIndex).type);
    if(it == infixParseFunctions.end())
    {
      FORGE_LOG_DEBUG("No Infix parser found for %s", input->at(tokenIndex).literal.c_str());
      return left;
    }
    Node*(*infix)(void*) = it->second;
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

  Node* value = nullptr;
  value = parseExpression(Precedence::LOWEST);
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

  if(match(OPEN_PARANTHESIS)){}
  FORGE_LOG_FATAL("Parsing an assignment statement");
  FORGE_LOG_TRACE("Position: %s", input->at(tokenIndex).literal.c_str());

  FORGE_LOG_WARNING("Token literal %s", token.literal.c_str());

  Node* rhs = nullptr;
  rhs = parseExpression(Precedence::LOWEST);
  tokenIndex++;

  Node* assigmentNode = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  initAssignmentNode(assigmentNode, variableName, rhs);
  return assigmentNode;
}

std::vector<FunctionParameter> parseFunctionParams()
{
  std::vector<FunctionParameter> identifiers;
  if (match(CLOSE_PARANTHESIS))
  {
    return identifiers;
  }

  while (true)
  {
    if(!match(IDENTIFIER))
    {
      FORGE_LOG_ERROR("Syntax error : expected an identifier");
      exit(1);
    }
    Node* currIden = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
    std::string currIdenName = input->at(tokenIndex-1).literal;
    initVariableNode(currIden, currIdenName);
    
    // - - - using placeholder variableContext for now
    FunctionParameter currIdenContext;
    currIdenContext.name = currIden->context.variableContext.name;
    identifiers.push_back(currIdenContext);
    if(match(COMMA)){}
    else
    {
      break;
    }
  }

  if(!match(CLOSE_PARANTHESIS))
  {
    FORGE_LOG_ERROR("Syntax error : expected a closing paranthesis");
    exit(1);
  }

  return identifiers;
}

// - - - FUNCTION parser

Node* parseFunctionLiteral(void* arg)
{
  tokenIndex++;
  std::vector<FunctionParameter> fnLit;
  if(!match(OPEN_PARANTHESIS))
  {
    FORGE_LOG_ERROR("Syntax Error: expected an Open Bracket in Function")
    exit(1);
  }
  
  fnLit = parseFunctionParams();
  
    if(!match(OPEN_BRACE))
  {
    FORGE_LOG_ERROR("Syntax Error: expected an Open Brace for expression in Function");  
    exit(1);
  }

  Block* fnBody = parseBlockStatement();
  for (u64 i = 0; i < program->allocator.allocated; i += sizeof(Node))
      {
        Node* node = (Node*)((char*)program->allocator.memory + i);
        FORGE_LOG_DEBUG(getNodeString(node).c_str());
      }
  initFunctionNode((Node*)arg, fnLit, fnBody);
  return (Node*)arg;
}

// - - - AST Creation Function
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
  registerPrefix(TRUE, parseBoolean);
  registerPrefix(FALSE, parseBoolean);
  registerInfix(EQUALS, parseInfixExpression);
  registerInfix(NOT_EQUALS, parseInfixExpression);
  registerInfix(LESSER, parseInfixExpression);
  registerInfix(GREATER, parseInfixExpression);
  registerInfix(LESSER_EQUAL, parseInfixExpression);
  registerInfix(GREATER_EQUAL, parseInfixExpression);
  registerInfix(ADD, parseInfixExpression);
  registerInfix(SUB, parseInfixExpression);
  registerInfix(BINARY_OPERATOR, parseInfixExpression);
  registerPrefix(OPEN_PARANTHESIS, parseGroupedExpress);
  registerPrefix(IF, parseIfExpression);
  registerPrefix(FUNCTION, parseFunctionLiteral);

  while (tokenIndex < input->size())
  {
    program->statements.push_back(parseStatement());
    tokenIndex++;
  }

  return true;
}
