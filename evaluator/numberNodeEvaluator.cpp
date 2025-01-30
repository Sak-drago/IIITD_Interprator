#include "../include/evaluator.h"

FORGE_INLINE DataType determineNumberType(const char* NUMBER_STRING)
{
  char* end;
  u8 base                   = 10;

  // - - - try signed int
  i64 intVal                = std::strtoll(NUMBER_STRING, &end, base);
  if (*end == '\0')  // - - - parse success
  {
    if (intVal >= std::numeric_limits<i8>::min() && 
        intVal <= std::numeric_limits<i8>::max())       return Int_8;

    if (intVal >= std::numeric_limits<i16>::min() && 
        intVal <= std::numeric_limits<i16>::max())      return Int_16;
    
    if (intVal >= std::numeric_limits<i32>::min() && 
        intVal <= std::numeric_limits<i32>::max())      return Int_32;
    
    if (intVal >= std::numeric_limits<i64>::min() && 
        intVal <= std::numeric_limits<i64>::max())      return Int_64;
  }

  // - - - try unsigned int
  u64 uIntVal               = std::strtoll(NUMBER_STRING, &end, base);
  if (*end == '\0')  // - - - parse success
  {
    if (uIntVal >= std::numeric_limits<u8>::min() && 
        uIntVal <= std::numeric_limits<u8>::max())      return UInt_8;

    if (uIntVal >= std::numeric_limits<u16>::min() && 
        uIntVal <= std::numeric_limits<u16>::max())     return UInt_16;
    
    if (uIntVal >= std::numeric_limits<u32>::min() && 
        uIntVal <= std::numeric_limits<u32>::max())     return UInt_32;
    
    if (uIntVal >= std::numeric_limits<u64>::min() && 
        uIntVal <= std::numeric_limits<u64>::max())     return UInt_64;
  }

  // - - - try double and float
  f64 fVal                  = std::strtod(NUMBER_STRING, &end);
  if (*end == '\0') // - - - parse succcess
  { 
    if (fVal   >= std::numeric_limits<f32>::min() && 
        fVal   <= std::numeric_limits<f32>::max())      return Float_32;

    if (fVal   >= std::numeric_limits<f64>::min() && 
        fVal   <= std::numeric_limits<f64>::max())      return Float_64;
  }

  // raiseSyntaxException();
  return Float_32; // -- - TODO: @Sakshat, find out why Float works
}

FORGE_INLINE void assign(const char* NUMBER_STRING, DataType TYPE, void* MEMORY)
{
  switch (TYPE)
  {
    case Int_8    : 
      *reinterpret_cast<i8*>(MEMORY)  = static_cast<i8>(std::stoi(NUMBER_STRING)); 
      break; 

    case UInt_8   : 
      *reinterpret_cast<u8*>(MEMORY)  = static_cast<u8>(std::stoul(NUMBER_STRING)); 
      break; 
    
    case Int_16   : 
      *reinterpret_cast<i16*>(MEMORY) = static_cast<i16>(std::stoi(NUMBER_STRING)); 
      break; 

    case UInt_16  : 
      *reinterpret_cast<u16*>(MEMORY) = static_cast<u16>(std::stoul(NUMBER_STRING)); 
      break; 
    
    case Int_32   : 
      *reinterpret_cast<i32*>(MEMORY) = static_cast<i32>(std::stoi(NUMBER_STRING)); 
      break; 

    case UInt_32  : 
      *reinterpret_cast<u32*>(MEMORY) = static_cast<u32>(std::stoul(NUMBER_STRING)); 
      break; 
    
    case Int_64   : 
      *reinterpret_cast<i64*>(MEMORY) = static_cast<i64>(std::stoll(NUMBER_STRING)); 
      break; 

    case UInt_64  : 
      *reinterpret_cast<u64*>(MEMORY) = static_cast<u64>(std::stoull(NUMBER_STRING)); 
      break; 
    
    case Float_32 : 
      *reinterpret_cast<f32*>(MEMORY) = static_cast<f32>(std::stof(NUMBER_STRING)); 
      break; 

    case Float_64 : 
      *reinterpret_cast<f64*>(MEMORY) = static_cast<f64>(std::stod(NUMBER_STRING)); 
      break;

    default :
      FORGE_ASSERT_MESSAGE(true, "Panic Assignmnet Failure");
  }
}

Data evaluateNumberNode(const Node* NUMBER_NODE)
{
  // - - - all these checks will dissapear in the release build
  FORGE_ASSERT_MESSAGE(NUMBER_NODE        != NULL,              "Cannot evaulate a NULL Number Node");
  FORGE_ASSERT_MESSAGE(NUMBER_NODE->type  == NODE_TYPE_NUMBER,  "A Node which is not a NODE_TYPE_NUMBER cannot be evaluated here");

  // - - - assign the value to the memory
  const char*       number      = NUMBER_NODE->context.numberContext.value;
  const DataType    numberType  = determineNumberType(number); 
  void*             value       = linearAllocatorAllocate(&(runtime.currentEnv->memory), getDataTypeSize(numberType)); 
  assign(number, numberType, value);

  // - - - return the data 
  return Data 
    {
      .type  = numberType,
      .value = value
    };
}
