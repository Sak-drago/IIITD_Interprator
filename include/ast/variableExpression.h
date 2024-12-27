#pragma once
#include "expression.h"

class VariableExpr : public Expression 
{
  public:
    VariableExpr(const std::string &NAME, u64 ADDRESS, u8 SIZE);

    const std::string name;
    const u8          size;
    std::string toString() const override;
};
