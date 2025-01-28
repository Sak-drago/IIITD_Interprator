#include "../include/data.h"
#include "../include/program.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <limits>

Program runtime;
// - - - All the eval functions - - -

// - - - number node
Data evaluateNumberNode(Node* NUMBER_NODE);


// - - - evaluate the Node - - - 

Data evaluate(Node* NODE)
{
  FORGE_ASSERT_MESSAGE(NODE != NULL, "Cannot evaluate a NULL node");
  return evaluateNumberNode(NODE);
}


// - - - Number Nodes - - -

DataType determineNumberType(const char* NUMBER_STRING)
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

  return Float_32; // -- - TODO: @Sakshat, find out why Float works
  FORGE_LOG_ERROR("Segfault");
  FORGE_ASSERT_MESSAGE(true,                                "Could not determine the Object Type for number");
  exit(1);
}

void assign(const char* NUMBER_STRING, DataType TYPE, void* MEMORY)
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
  }
}


void* allocate(const char* NUMBER_STRING, Environment* ENVIRONMENT)
{
  FORGE_ASSERT_MESSAGE(NUMBER_STRING != NULL, "Cannot allocate 'NULL' string, use NULL ObjectType");
  FORGE_ASSERT_MESSAGE(ENVIRONMENT   != NULL, "Cannot allocate in a NULL Environment");

  DataType  type            = determineNumberType(NUMBER_STRING);
  u8        allocationSize; 
  switch (type) 
  {
    default       :   ; // - - - TODO: handle other datatypes

    case Int_8    :
    case UInt_8   : 
    case Bool     :   allocationSize = 1; break;  // - - - 8 bit (1 byte) number
    
    case Int_16   :
    case UInt_16  :   allocationSize = 2; break;  // - - - 16 bit (2 byte) number
    
    case Int_32   :
    case UInt_32  :
    case Float_32 :   allocationSize = 4; break;  // - - - 32 bit (4 byte) number
    
    case Int_64   :
    case UInt_64  :
    case Float_64 :   allocationSize = 64; break; // - - - 64 bit (8 byte) number
  }
  
  FORGE_LOG_TRACE("Allocating %d bytes for %s of type %s", allocationSize, NUMBER_STRING, getDataTypeStr(&type).c_str());
  void* numberMemory = linearAllocatorAllocate(&ENVIRONMENT->memory, allocationSize); 
  FORGE_ASSERT_MESSAGE(numberMemory, "Failed to allocate memory");
  assign(NUMBER_STRING, type, numberMemory);
  return numberMemory;
}


Data evaluateNumberNode(Node* NUMBER_NODE)
{
  FORGE_ASSERT_MESSAGE(NUMBER_NODE        != NULL,              "Cannot evaulate a NULL Number Node");
  FORGE_ASSERT_MESSAGE(NUMBER_NODE->type  == NODE_TYPE_NUMBER,  "A Node which is not a NODE_TYPE_NUMBER cannot be evaluated here");

  const char*       number      = NUMBER_NODE->context.numberContext.value;
  const DataType    numberType  = determineNumberType(number); 
  const void*       value       = allocate(number, &runtime.global); // - - - TODO: determine which environment to use
  
  return Data 
    {
      .type  = numberType,
      .value = value
    };
}
