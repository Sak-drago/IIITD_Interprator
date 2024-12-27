#include "../../include/ast/immediateExpression.h"

ImmediateExpr::ImmediateExpr(u64 value) 
{
  this->value = value;
}

std::string ImmediateExpr::toString() const 
{
  std::string retVal = "{ type : [IMMEDIATE_EXPRESSION] \t\t value : ";
  return retVal + std::to_string(value) + " }";
}
