#pragma once
#include "expression.h"

typedef enum Operator
{
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  POWER,
  REMAINDER,
} Operator;

class BinaryExpr : public Expression 
{
  private:

    const Expression    *left;
    const Expression    *right;
    const Operator      operation;

  public:
    BinaryExpr(const Expression* LEFT, const Expression* RIGHT, Operator OPERATION);
    std::string toString() const override;
};
