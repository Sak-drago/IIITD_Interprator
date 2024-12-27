#include "../../include/ast/variableExpression.h"
#include <string>

VariableExpr::VariableExpr(const std::string& NAME, u64 ADDRESS, u8 SIZE) 
 : name(NAME), size(SIZE) 
{
  this->value = ADDRESS;
}

std::string VariableExpr::toString() const 
{
  std::string retVal = "{ type : [VARIABLE_EXPRESSION] \t\t address : ";
  retVal += std::to_string(value) + " \t size : ";
  retVal += std::to_string(size) + " }";
  return retVal;
}
