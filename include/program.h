#pragma once 
#include "../defines.h"
#include "../library/include/linearAlloc.h"
#include "ast.h"
#include <unordered_map>
#include <vector>

typedef struct Environment
{
  LinearAllocator                         memory;
  std::unordered_map<std::string, void*>  pointers;
} Environment;

typedef struct Program
{
  std::vector<Node*>  statements;
  LinearAllocator     allocator;
  Environment         global;
  Environment         stack;
} Program;

extern Program runtime;
