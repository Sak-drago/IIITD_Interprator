#pragma once 
#include "../defines.h"
#include "../library/include/linearAlloc.h"
#include "ast.h"
#include <vector>

typedef struct Program
{
  std::vector<Node*>  statements;
  LinearAllocator     allocator;
  LinearAllocator     globalMemory;
  LinearAllocator     stack;
} Program;
