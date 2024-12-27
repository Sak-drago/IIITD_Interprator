#pragma once
#include "expression.h"

class ImmediateExpr : public Expression 
{
  public:
    ImmediateExpr(u64 VALUE);
    std::string toString() const override;
};
