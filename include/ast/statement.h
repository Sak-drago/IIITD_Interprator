#pragma once
#include "../../defines.h"
#include <string>

class Statement 
{
  public:
    // TODO:@Sakshat will rewrite all the toString to be pretty
    virtual std::string toString() const = 0; 
};
