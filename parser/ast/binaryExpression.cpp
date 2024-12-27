#include "../../include/ast/binaryExpression.h"

BinaryExpr::BinaryExpr(const Expression* LEFT, const Expression* RIGHT, Operator OPERATION)
  : 
  left(LEFT), 
  right(RIGHT), 
  operation(OPERATION) 
  {}

std::string BinaryExpr::toString() const 
{
  std::string op;
  switch (operation) 
  {
    case ADDITION: 
      op = "+";
      break;
    
    case SUBTRACTION: 
      op = "-";
      break;

    case MULTIPLICATION: 
      op = "*";
      break;

    case DIVISION: 
      op = "/";
      break;

    case REMAINDER: 
      op = "%";
      break;

    case POWER: 
      op = "^";
      break;
  }
  return "{ type : [BINARY_EXPRESSION] \t\tleft: " + left->toString() + " " + op + " right: " + right->toString() + "}";
}
