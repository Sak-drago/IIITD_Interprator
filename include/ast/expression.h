#pragma once
#include "statement.h"

class Expression : public Statement 
{
  public:
    u64 value;
};
