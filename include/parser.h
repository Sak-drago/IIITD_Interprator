#include "../defines.h"
#include "tokens.h"
#include <vector>
#include "program.h"
#include "ast/binaryExpression.h"
#include "ast/variableExpression.h"
#include "ast/immediateExpression.h"

class Parser 
{
  private:
    std::vector<Token>  tokens;
    u64                 index; 

    Token               peek() const;
    Token               advance();
    bool                match(TokenType TYPE);
    Token               consume(TokenType TYPE, const std::string& EXCEPTION);

    Statement*          parseStatement();
    Expression*         parseExpression();
    Expression*         parsePrimary();
    BinaryExpr*         parseBinary(Expression* LEFT);
    VariableExpr*       parseVariable();
    ImmediateExpr*      parseImmediate();

  public:
    Parser(const std::vector<Token>& TOKENS);
    Program parse();
};
