#include "../include/evaluator.h"
#include <cmath>


FORGE_INLINE bool areTypeCompatible(DataType TYPE_1, DataType TYPE_2)
{
  if (TYPE_1 == TYPE_2) return true;
  switch (TYPE_1)
  {
    // - - - numbers
    case Int_8    :
    case Int_16   :
    case Int_32   :
    case Int_64   :
    case UInt_8   :
    case UInt_16  :
    case UInt_32  : 
    case UInt_64  :
    case Float_32 :
    case Float_64 :
      switch (TYPE_2)
      {
        case Int_8    :
        case Int_16   :
        case Int_32   :
        case Int_64   :
        case UInt_8   :
        case UInt_16  :
        case UInt_32  : 
        case UInt_64  :
        case Float_32 :
        case Float_64 : return true;
        default       : return false; 
      }
    case Bool:
      switch (TYPE_2)
      {
        case Bool     : return true;
        default       : return false;
      }
    default     : return false;
  }
  return false;
}


FORGE_INLINE DataType getResultingDataType (DataType TYPE_1, DataType TYPE_2, BinaryOperator OPCODE)
{
  switch (OPCODE)
  {
    case COMPARISON_OPERATOR_EQUAL          :
    case COMPARISON_OPERATOR_LESSER         :
    case COMPARISON_OPERATOR_GREATER        :
    case COMPARISON_OPERATOR_LESSER_EQUAL   :
    case COMPARISON_OPERATOR_NOT_EQUAL      :
    case COMPARISON_OPERATOR_GREATER_EQUAL  : return Bool;
    case BINARY_OPERATOR_REMAINDER          : return Int_32;
    case BINARY_OPERATOR_POWER              : return Float_64;
    default :
      switch (TYPE_1)
      {
        case Bool     : return Bool; 
        case Float_64 : return Float_64;
        case Float_32 : 
          switch (TYPE_2)
          {
            case Float_64 : return Float_64;
            default       : return Float_32;
          }
        case Int_64   : 
          switch (TYPE_2)
          {
            case Float_64 : return Float_64;
            case Float_32 : return Float_32;
            default       : return Int_64;
          }
        case UInt_64  : 
          switch (TYPE_2)
          {
            case Float_64 : return Float_64;
            case Float_32 : return Float_32;
            case UInt_64  : return UInt_64;
            default       : return Int_64;  
          }
        case UInt_32  : 
        case UInt_16  : 
        case UInt_8   :
          switch (TYPE_2) 
          {
            case Float_64 : return Float_64;
            case Float_32 : return Float_32;
            case Int_64   : return Int_64;
            case UInt_64  : return UInt_64;
            case UInt_32  : 
            case UInt_16  :
            case UInt_8   : return UInt_32;
            default       : return Int_32;  
          }
        default       : 
          switch (TYPE_2) 
          {
            case Float_64 : return Float_64;
            case Float_32 : return Float_32;
            case Int_64   : return Int_64;
            default       : return Int_32;  
          }
      }
  }
}


Data evaluateBinaryNode(const Node* BINARY_NODE)
{
  // - - - all these checks will disappear in the release build
  FORGE_ASSERT_MESSAGE(BINARY_NODE != NULL, "Cannot evaluate a NULL Binary Operator Node");
  FORGE_ASSERT_MESSAGE(BINARY_NODE->type == NODE_TYPE_BINARY_OPERATOR, "A Node which is not a NODE_TYPE_BINARY_OPERATOR cannot be evaluated here");

  // - - - get the values of the left and right Data
  Data left  = evaluate(BINARY_NODE->context.binaryContext.left);
  Data right = evaluate(BINARY_NODE->context.binaryContext.right);
  
  if (!areTypeCompatible(left.type, right.type)) 
      raiseException("Types are nt compatible for binary operations");

  // - - - TODO: Garbage collection might want to free the value of left and right

  BinaryOperator  op              = BINARY_NODE->context.binaryContext.opcode;
  DataType        resultingType   = getResultingDataType(left.type, right.type, op);
  void*           memory          = linearAllocatorAllocate(&runtime.currentEnv->memory, getDataTypeSize(resultingType));

  // - - - Macros - - - 

  // - - - get the result type 
  #define RESULT_TYPE(LTYPE, RTYPE, OP) decltype(std::declval<LTYPE>() OP std::declval<RTYPE>())

  // - - - handle arithmetic operations 
  #define HANDLE_ARITHMETIC_OP(OPCODE, LTYPE, RTYPE, LVAL, RVAL, OP) \
    case OPCODE: \
    { \
      using ResultType        = RESULT_TYPE(LTYPE, RTYPE, OP); \
      *((ResultType*)memory)  = (LVAL OP RVAL); \
      break; \
    }
  
  #define HANDLE_REMAINDER(OPCODE, LTYPE, RTYPE, LVAL, RVAL, OP) \
    case OPCODE: \
    { \
      *((i32*)memory)  = ((i32)LVAL OP (i32)RVAL); \
      break; \
    }
  
  #define HANDLE_POW(OPCODE, LTYPE, RTYPE, LVAL, RVAL, OP) \
    case OPCODE: \
    { \
      *((f64*)memory)  = std::pow(LVAL, RVAL); \
      break; \
    }

  // - - - handle comparison operations 
  #define HANDLE_COMPARISON_OP(OPCODE, LTYPE, RTYPE, LVAL, RVAL, OP) \
    case OPCODE: *((bool*)memory) = (bool)(LVAL OP RVAL); break;

  // - - - handle all operations
  #define HANDLE_OP(LTYPE, RTYPE) \
    switch (op) \
    { \
      HANDLE_REMAINDER    (BINARY_OPERATOR_REMAINDER,           LTYPE, RTYPE, lVal, rVal,  %) \
      HANDLE_POW          (BINARY_OPERATOR_POWER,               LTYPE, RTYPE, lVal, rVal,  ^) \
      HANDLE_ARITHMETIC_OP(BINARY_OPERATOR_ADDITION,            LTYPE, RTYPE, lVal, rVal,  +) \
      HANDLE_ARITHMETIC_OP(BINARY_OPERATOR_SUBTRACTION,         LTYPE, RTYPE, lVal, rVal,  -) \
      HANDLE_ARITHMETIC_OP(BINARY_OPERATOR_MULTIPLICATION,      LTYPE, RTYPE, lVal, rVal,  *) \
      HANDLE_ARITHMETIC_OP(BINARY_OPERATOR_DIVISION,            LTYPE, RTYPE, lVal, rVal,  /) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_GREATER,         LTYPE, RTYPE, lVal, rVal,  <) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_LESSER,          LTYPE, RTYPE, lVal, rVal,  >) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_EQUAL,           LTYPE, RTYPE, lVal, rVal, ==) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_GREATER_EQUAL,   LTYPE, RTYPE, lVal, rVal, <=) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_LESSER_EQUAL,    LTYPE, RTYPE, lVal, rVal, >=) \
      HANDLE_COMPARISON_OP(COMPARISON_OPERATOR_NOT_EQUAL,       LTYPE, RTYPE, lVal, rVal, !=) \
      default: raiseException("Unsupported binary operation between " #LTYPE " and " #RTYPE);\
    }

  // - - - handle r type switches 
  #define HANDLE_BINARY_OP(LTYPE) \
    switch (right.type) \
    { \
      case Int_8     :     {i8  rVal = *((i8*)right.value);  HANDLE_OP(LTYPE, i8);  break;} \
      case Int_16    :     {i16 rVal = *((i16*)right.value); HANDLE_OP(LTYPE, i16); break;} \
      case Int_32    :     {i32 rVal = *((i32*)right.value); HANDLE_OP(LTYPE, i32); break;} \
      case Int_64    :     {i64 rVal = *((i64*)right.value); HANDLE_OP(LTYPE, i64); break;} \
      case UInt_8    :     {i8  rVal = *((i8*)right.value);  HANDLE_OP(LTYPE, u8);  break;} \
      case UInt_16   :     {i16 rVal = *((i16*)right.value); HANDLE_OP(LTYPE, u16); break;} \
      case UInt_32   :     {i32 rVal = *((i32*)right.value); HANDLE_OP(LTYPE, u32); break;} \
      case UInt_64   :     {i64 rVal = *((i64*)right.value); HANDLE_OP(LTYPE, u64); break;} \
      case Float_32  :     {f32 rVal = *((f32*)right.value); HANDLE_OP(LTYPE, f32); break;} \
      case Float_64  :     {f64 rVal = *((f64*)right.value); HANDLE_OP(LTYPE, f64); break;} \
      default: raiseException("Operation on " #LTYPE " and a non-number Node is undefined");\
    } \
    break;


  switch (left.type)
  {
    case Bool:
    {
      bool lVal = *((bool*)left.value);
      switch (right.type)
      {
        case Bool : 
          {
            bool rVal = *((bool*)right.value);
            HANDLE_OP(bool, bool);
          }
        default : raiseException("Cannot perform any operation on a Bool and not Bool");  
      }
      break;
    }
    case Int_8    : { i8 lVal  = *((i8*)left.value);  HANDLE_BINARY_OP(i8) ; }
    case Int_16   : { i16 lVal = *((i16*)left.value); HANDLE_BINARY_OP(i16); }
    case Int_32   : { i32 lVal = *((i32*)left.value); HANDLE_BINARY_OP(i32); }
    case Int_64   : { i64 lVal = *((i64*)left.value); HANDLE_BINARY_OP(i64); }
    case UInt_8   : { u8 lVal  = *((u8*)left.value);  HANDLE_BINARY_OP(u8) ; }
    case UInt_16  : { u16 lVal = *((u16*)left.value); HANDLE_BINARY_OP(u16); }
    case UInt_32  : { u32 lVal = *((u32*)left.value); HANDLE_BINARY_OP(u32); }
    case UInt_64  : { u64 lVal = *((u64*)left.value); HANDLE_BINARY_OP(u64); }
    case Float_32 : { f32 lVal = *((f32*)left.value); HANDLE_BINARY_OP(f32); }
    case Float_64 : { f64 lVal = *((f64*)left.value); HANDLE_BINARY_OP(f64); }
    default : raiseException("Operation not defined between a number and a non-number");
  }

  Data retDat;
  retDat.type = resultingType;
  retDat.value = memory;

  return retDat;
}
