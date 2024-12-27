#pragma once
#include "ast/statement.h"
#include <vector>

typedef struct Program 
{
  std::vector<Statement> statements;
  u64 index;
} Program;
