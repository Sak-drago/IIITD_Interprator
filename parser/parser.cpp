#include "../include/ast.h"
#include "../include/parser.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../include/tokenizer.h"
#include "../include/tokens.h"
#include <vector>
#include <bits/algorithmfwd.h>

static Program*              program      = NULL;
static std::vector<Token>*   input        = NULL;
static u64                   tokenIndex   = 0;


// - - -  Prefix and Infix Functions  - - -

// - - - maps to store the functions from token types to parse functions
std::unordered_map<TokenType, Node*(*)(void*)>  prefixParseFunctions;
std::unordered_map<TokenType, Node*(*)(void*)>  infixParseFunctions;


// - - - add a prefix or infix
void registerPrefix(TokenType TYPE, Node* (*PARSE_FUNCTIONS)(void*))
{
  prefixParseFunctions[TYPE] = PARSE_FUNCTIONS;
}
void registerInfix(TokenType type, Node* (*parseFunction)(void*))
{
  infixParseFunctions[type] = parseFunction;
}


// - - - Precedence order - - -

// - - - TODO: @Asher: Just make the precedence equal. Right now, ">" > "<".
typedef enum Precendence
{
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

// - - - Checks what Precedence does the current token have
int curPrecedence()
{
  if (input->size() <= tokenIndex)    return LOWEST;
  Token token                         = input->at(tokenIndex);

  if (precedencesMapping.find(token.type) != precedencesMapping.end())
  {
    if(token.type == BINARY_OPERATOR)
    {
      switch (token.literal[0])
      {
        case '+':
        case '-': return SUM;
        case '*':
        case '/': return PRODUCT;
        default : return precedencesMapping[token.type];
      }
    }
    return precedencesMapping[token.type];
  }

  return LOWEST;
}

// - - - Checks what Precedence does the next token have
int peekPrecedence()
{
  if (input->size() <= tokenIndex+1)    return LOWEST;
  Token token                           = input->at(tokenIndex+1);

  if (precedencesMapping.find(token.type) != precedencesMapping.end())
  {    
    return precedencesMapping[token.type];  
  }

  return LOWEST;
}


// - - - Helper functions - - -

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
    if(token.literal == "<")  return COMPARISON_OPERATOR_GREATER;
    if(token.literal == ">")  return COMPARISON_OPERATOR_LESSER;
    if(token.literal == "==") return COMPARISON_OPERATOR_EQUAL;
    if(token.literal == "<=") return COMPARISON_OPERATOR_GREATER_EQUAL;
    if(token.literal == "=>") return COMPARISON_OPERATOR_LESSER_EQUAL;
    if(token.literal == "!=") return COMPARISON_OPERATOR_NOT_EQUAL;
    FORGE_LOG_ERROR("How did you even get here Asher but for Comparison?");
  }
  FORGE_LOG_ERROR("How did you even get here Asher? Part 2, %s, %i", token.literal.c_str(), tokenIndex);
  exit(1);
}

void raiseSynaxError(TokenType EXPECTED_TYPE)
{ 
  FORGE_LOG_FATAL("Syntax error : at token index %d, expected a type of %s but recieved %s", 
                  tokenIndex, 
                  getTokenTypeString(EXPECTED_TYPE).c_str(), 
                  getTokenTypeString(input->at(tokenIndex).type).c_str());
  // - - - TODO: do something more than printing
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
  return false;
}


// - - - Parsing Methods - - -

// - - - Statement
Node* parseStatement()
{
  FORGE_ASSERT_MESSAGE(input != NULL, "Cannot begin parsing before recieveing input");

  Token token = input->at(tokenIndex); 
  Token tokenPeak = input->at(tokenIndex+1);
  switch (token.type)
  {
    case PLAG      :     return parseAssignmentExpression();
    case RETURN    :     return parseReturnStatement();
    case IDENTIFIER:  
      if(tokenPeak.type == ASSIGN) return parseNormalAssignmentExpression();
      else return parseExpression(LOWEST);
    default        :     return parseExpression(LOWEST);
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
    FORGE_LOG_DEBUG("Parsed a block statement at index %d, token is %s, token name is %s", tokenIndex, getTokenTypeString(input->at(tokenIndex).type).c_str(), input->at(tokenIndex).literal.c_str());
    if(input->at(tokenIndex).type == NUMBER)  tokenIndex++;
  }
  return block;
}

// - - - Prefix
// - - - NOTE: Approach followed for parsing an expression is the "PRATT PARSER" approach.
Node* parsePrefixExpression(void* ARG) 
{
  Token token = input->at(tokenIndex);
  if(ARG == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Prefix Node");
    exit(1);
  }
  std::string operatorType = token.literal;

  // - - - TODO: @Asher: Only supports "!" operator for now. Add support for "-" operator.
  Node* right = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  right = parseExpression(Precedence::NOT_PRECEDENCE);
  initPrefixNode((Node*) ARG, operatorType.c_str(), right);
  return (Node*)ARG;
}

// - - - Infix
Node* parseInfixExpression(void* ARG)
{
  FORGE_ASSERT_MESSAGE(ARG != NULL, "Cannot initialize a NULL AST Binary Operator Node");

  // - - - allocate a node 
  Node* left            = (Node*)linearAllocatorAllocate(&program->allocator, sizeof(Node));
  Token token           = input->at(tokenIndex);

  // - - - get the opcode, precedence and move to the next token
  BinaryOperator opcode = getBinaryOperator(token);
  int precedence        = curPrecedence();
  tokenIndex++;
  //-- If we have a '(' consume it
  match(OPEN_PARANTHESIS);

  // - - - parse the expression on the right. If we have a ')' consume it
  Node* right           = parseExpression(precedence);
  match(CLOSE_PARANTHESIS);

  // - - - initialize the binary operator node
  initBinaryOpNode(left, (Node*)ARG , right, opcode);
  return left;
}

// - - - identifier
Node* parseIdentifier(void* ARG)
{
  Token token = input->at(tokenIndex);
  if(ARG == NULL)
  {
    FORGE_LOG_ERROR("Cannot initialize a NULL AST Variable Node");
    exit(1);
  }
  std::string variName = token.literal;
  initVariableNode((Node*) ARG, variName);
  return (Node*)ARG;
}

// - - - integer
Node* parseInteger(void* ARG)
{
  FORGE_ASSERT_MESSAGE(ARG != NULL, "Cannot initialize a null AST number node");

  FORGE_LOG_DEBUG("Parsing a number");
  Token token = input->at(tokenIndex);
  std::string number = token.literal;
  initNumberNode((Node*) ARG, number);
  if (tokenIndex + 1 < input->size() && input->at(tokenIndex + 1).type == CLOSE_PARANTHESIS) tokenIndex++;
  return (Node*)ARG;
}

Node* parseBoolean(void* ARG)
{
  Token token = input->at(tokenIndex);
  FORGE_LOG_TRACE("Parsing a boolean value");
  if(token.type == TRUE)
  { 
    initBoolNode((Node*)ARG, true);
    return (Node*)ARG;
  }
  else if(token.type == FALSE)
  {
    initBoolNode((Node*)ARG, false);
    return (Node*)ARG;
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
  Node* expression                = parseExpression(Precedence::LOWEST);
  if (!match(CLOSE_PARANTHESIS))  raiseSynaxError(CLOSE_PARANTHESIS);
  return expression;
}

// - - - Only supports "IF" with/without "ELSE" statement for now
Node* parseIfExpression(void* arg)
{
  // - - - match if and open parenthesis. Close is managed by parseEXpression
  if (!match(IF))               raiseSynaxError(IF);
  if (!match(OPEN_PARANTHESIS)) raiseSynaxError(OPEN_PARANTHESIS);

  // - - - get the condition
  Node* condition               = parseExpression(Precedence::LOWEST);
  // - - - skip number, variable, bool and close paranthesis
  if(input->at(tokenIndex).type == IDENTIFIER || input->at(tokenIndex).type == NUMBER) tokenIndex+=2;

  // - - - get the body of the if statement in braces
  if (!match(OPEN_BRACE))       raiseSynaxError(OPEN_BRACE);
  Block* consequence            = parseBlockStatement();

  // - - - check if we have an else condition
  tokenIndex++;
  Block* alternative            = nullptr;
  if (tokenIndex < input->size() && match(ELSE))
  {
    if (!match(OPEN_BRACE))     raiseSynaxError(OPEN_BRACE);
    alternative                 = parseBlockStatement();
  }

  // - - - make the if node
  initIfNode((Node*)arg, condition, consequence, alternative);
  return (Node*)arg;
}

// - - - expression
Node* parseExpression(int PRECEDENCE)
{
  // - - - see the current token and allocate a left node
  Token token                             = input->at(tokenIndex);
  Node* left                              = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  
  // - - - see if we have a valid prefix parsing function
  auto  it                                = prefixParseFunctions.find(token.type);
  if (it == prefixParseFunctions.end())   raiseSynaxError(input->at(tokenIndex).type);  
  else
  {
    Node*(*prefix)(void*)                 = it->second;           // - - - here prefix is the correct function we need to parse the prefix
    left                                  = prefix((void*)left);  // - - - call the prefix function
  }

  // - - - parse the infix now
  while (PRECEDENCE < peekPrecedence())
  {
    tokenIndex++;
    auto it                               = infixParseFunctions.find(input->at(tokenIndex).type);
    if (it == infixParseFunctions.end())                          // - - - if the function doesnt exist, we can simply return the left node
    {
      FORGE_LOG_DEBUG("No Infix parser found for %s", input->at(tokenIndex).literal.c_str());
      return left;
    }
    Node*(*infix)(void*)                  = it->second;           // - - - here, infix is the correct infix parsing function
    left                                  = infix((void*)left);   // - - - call the infix function
  }

  return left;
}

// - - - return statments
Node* parseReturnStatement()
{
  // - - - Match whether we have the syntax :   Return <var>
  if (!match(RETURN))             raiseSynaxError(RETURN);

  // - - - parse the value and and move on to the next token
  match(OPEN_PARANTHESIS);        // - - - if we start with '(', consume it.
  // NOTE: @Sakshat, this match freed one memory leak for statement "return (1)". We were calling parse expression once more on OPEN_PARANTHESIS
  Node* value                     = parseExpression(Precedence::LOWEST);
  tokenIndex++;
 
  // - - - allocate and set the return node
  Node* returnNode                = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  initReturnNode(returnNode, value);
  return returnNode;
}

// - - - assignment expression
Node* parseAssignmentExpression()
{
  // - - - Match whether we have the syntax:  Plag <var>
  if (!match(PLAG))             raiseSynaxError(PLAG);
  if (!match(IDENTIFIER))       raiseSynaxError(IDENTIFIER);
 
  // - - - get the variable name
  Token       var               = input->at(tokenIndex-1);
  std::string variableName      = input->at(tokenIndex-1).literal;

  // - - - Match whether we have the synax: = <rhs>
  if (!match(ASSIGN))           raiseSynaxError(ASSIGN);
       match(OPEN_PARANTHESIS); // - - - handle the case of a paranthesis '('

  // - - - construct the rhs
  Node* rhs                     = parseExpression(Precedence::LOWEST); 
  tokenIndex++;

  // - - - allocate and return the assignmnet Node
  Node* assigmentNode           = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  initAssignmentNode(assigmentNode, variableName, rhs, 1);
  return assigmentNode;
}

Node* parseNormalAssignmentExpression()
{
  // - - - Match whether we have the syntax: <var>
  if (!match(IDENTIFIER))       raiseSynaxError(IDENTIFIER);
 
  // - - - get the variable name
  Token       var               = input->at(tokenIndex-1);
  std::string variableName      = input->at(tokenIndex-1).literal;

  // - - - Match whether we have the synax: = <rhs>
  if (!match(ASSIGN))           raiseSynaxError(ASSIGN);
       match(OPEN_PARANTHESIS); // - - - handle the case of a paranthesis '('

  // - - - construct the rhs
  Node* rhs                     = parseExpression(Precedence::LOWEST); 
  tokenIndex++;

  // - - - allocate and return the assignmnet Node
  Node* assigmentNode           = (Node*) linearAllocatorAllocate(&program->allocator, sizeof(Node));
  initAssignmentNode(assigmentNode, variableName, rhs, 0);
  return assigmentNode;
}


// - - - Functions
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

Node* parseFunctionLiteral(void* arg)
{
  tokenIndex++;
  for(auto itr = program->functionDefined.begin(); itr!= program->functionDefined.end(); itr++)
  {
    FORGE_LOG_DEBUG("CHECKING FOR FUNCTION DEFINED BEFORE %s with %s", (*itr)->context.functionContext.name, input->at(tokenIndex).literal.c_str());
    if (strcmp((*itr)->context.functionContext.name, input->at(tokenIndex).literal.c_str()) == 0)
    {
      FORGE_LOG_DEBUG("FOUND FUCNTION DEFINED BEFORE");
      while(input->at(tokenIndex).type != CLOSE_PARANTHESIS|| tokenIndex+1 >= input->size())
      {
        tokenIndex++;
      }
      tokenIndex++;
      if(!match(OPEN_BRACE))
      {
        FORGE_LOG_ERROR("Syntax Error: Cannot redeclare twice");
        exit(1);
      }
      (*itr)->context.functionContext.body = parseBlockStatement();
      return (Node*)*itr;
    }
  }

  if(!match(IDENTIFIER))
  {
    FORGE_LOG_ERROR("Syntax Error: expected an Identifier for Function");
    exit(1);
  }
  std::string fnName = input->at(tokenIndex-1).literal;
  std::vector<FunctionParameter> fnLit;
  if(!match(OPEN_PARANTHESIS))
  { 
    FORGE_LOG_ERROR("Syntax Error: expected an Open Bracket in Function")
    exit(1);
  }
  
  fnLit = parseFunctionParams();
  
  if(!match(OPEN_BRACE))
  {
    if(tokenIndex+1 >= input->size() || input->at(tokenIndex+1).type != CLOSE_BRACE)
    {
      Block* fnBody = nullptr;
      initFunctionNode((Node*)arg, fnName.c_str(), fnLit, fnBody);
      program->functionDefined.push_back(((Node*)arg));
      tokenIndex--;
      return (Node*) arg;
    }
    FORGE_LOG_ERROR("Syntax Error: expected an Open Brace for expression in Function");  
    exit(1);
  }

  Block* fnBody = parseBlockStatement();
  initFunctionNode((Node*)arg, fnName.c_str(), fnLit, fnBody);
  program->functionDefined.push_back((Node*)arg);
  return (Node*)arg;
}

// - - - AST Creation Function
void produceAST(std::vector<Token>* TOKENS, Program* PROGRAM)
{
  FORGE_ASSERT_MESSAGE(TOKENS   != NULL,  "TOKENS cannot be NULL to construct a program");
  FORGE_ASSERT_MESSAGE(PROGRAM  != NULL,  "Program cannot be NULL to construct a program");

  input                 = TOKENS;
  program               = PROGRAM;
  tokenIndex            = 0;
  static bool firstTime = true;

  if (firstTime)
  {
    FORGE_LOG_INFO("Registering parse functions");

    // - - - Registering the parse functions - - - 

    // - - - prefixes
    registerPrefix(NOT,               parsePrefixExpression);
    registerPrefix(IDENTIFIER,        parseIdentifier);
    registerPrefix(NUMBER,            parseInteger);
    registerPrefix(TRUE,              parseBoolean);
    registerPrefix(FALSE,             parseBoolean);
    registerPrefix(OPEN_PARANTHESIS,  parseGroupedExpress);
    registerPrefix(IF,                parseIfExpression);
    registerPrefix(FUNCTION,          parseFunctionLiteral);

    // - - - infixes
    registerInfix(EQUALS,             parseInfixExpression);
    registerInfix(NOT_EQUALS,         parseInfixExpression);
    registerInfix(LESSER,             parseInfixExpression);
    registerInfix(GREATER,            parseInfixExpression);
    registerInfix(LESSER_EQUAL,       parseInfixExpression);
    registerInfix(GREATER_EQUAL,      parseInfixExpression);
    registerInfix(ADD,                parseInfixExpression);
    registerInfix(SUB,                parseInfixExpression);
    registerInfix(BINARY_OPERATOR,    parseInfixExpression);

    firstTime = false;
  }

  while (tokenIndex < input->size())
  {
    program->statements.push_back(parseStatement());
    tokenIndex++;
  }
}
