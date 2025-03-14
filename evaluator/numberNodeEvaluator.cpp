#include "../include/evaluator.h"
#include <string>

static i32 anonymousVarCount = 0;

FORGE_INLINE DataType determineNumberType(const char* NUMBER_STRING)
{
  char* end;
  u8 base                   = 10;

  // - - - try signed int
  i64 intVal                = std::strtoll(NUMBER_STRING, &end, base);
  if (*end == '\0')  // - - - parse success
  {
    if (intVal >= std::numeric_limits<i64>::min() && 
        intVal <= std::numeric_limits<i64>::max())      return Integer;
  }

  // - - - try double and float
  f64 fVal                  = std::strtod(NUMBER_STRING, &end);
  if (*end == '\0') // - - - parse succcess
  { 
    if (fVal   >= std::numeric_limits<f64>::min() && 
        fVal   <= std::numeric_limits<f64>::max())      return Float;
  }

  // raiseSyntaxException();
  return Float; // -- - TODO: @Sakshat, find out why Float works
}

FORGE_INLINE void assign(const char* NUMBER_STRING, DataType TYPE, void* MEMORY)
{
  switch (TYPE)
  {    
    case Integer    : 
      *reinterpret_cast<i64*>(MEMORY) = static_cast<i64>(std::stoll(NUMBER_STRING)); 
      break; 

    case Float      : 
      *reinterpret_cast<f64*>(MEMORY) = static_cast<f64>(std::stod(NUMBER_STRING)); 
      break;

    default : raiseException("Number Type Evaluation Assignment Failed");
  }
}

Data evaluateNumberNode(const Node* NUMBER_NODE, const char* NAME)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(NUMBER_NODE        != NULL,              "Cannot evaulate a NULL Number Node");
  FORGE_ASSERT_MESSAGE(NUMBER_NODE->type  == NODE_TYPE_NUMBER,  "A Node which is not a NODE_TYPE_NUMBER cannot be evaluated here");

  // - - - assign the value to the memory
  const char*       number      = NUMBER_NODE->context.numberContext.value;
  const DataType    numberType  = determineNumberType(number); 
  std::string       name        = NAME ? std::string(NAME) : "anonymous_number_" + std::to_string(anonymousVarCount++);
  Data* pointer                 = createVariable(name, numberType);
  assign(number, numberType, pointer->memory);
  return *pointer;
}
