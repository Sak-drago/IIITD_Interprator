#include "../include/evaluator.h"


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
    default     : return false;
  }
  return false;
}


FORGE_INLINE DataType getResultingDataType (DataType TYPE_1, DataType TYPE_2)
{
  switch (TYPE_1)
  {
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


Data evaluateBinaryNode(const Node* BINARY_NODE)
{
  // - - - all these checks will disappear in the release build
  FORGE_ASSERT_MESSAGE(BINARY_NODE != NULL, "Cannot evaluate a NULL Binary Operator Node");
  FORGE_ASSERT_MESSAGE(BINARY_NODE->type == NODE_TYPE_BINARY_OPERATOR, "A Node which is not a NODE_TYPE_BINARY_OPERATOR cannot be evaluated here");

  // - - - get the values of the left and right Data
  Data left  = evaluate(BINARY_NODE->context.binaryContext.left);
  Data right = evaluate(BINARY_NODE->context.binaryContext.right);
  
  if (!areTypeCompatible(left.type, right.type)) 
      raiseException();

  // - - - TODO: Garbage collection might want to free the value of left and right

  DataType resultingType = getResultingDataType(left.type, right.type);
  void* memory = linearAllocatorAllocate(&runtime.currentEnv->memory, getDataTypeSize(resultingType));
  BinaryOperator op = BINARY_NODE->context.binaryContext.opcode;

  #define HANDLE_OP(OPCODE, TYPE) \
      case OPCODE: *((decltype(TYPE)*)memory) = TYPE; break;

  switch (left.type)
  {
    case Int_8:
    {
      i8 lVal = *((i8*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default: raiseException();
      }
      break;
    }
    case Int_16:
    {
      i16 lVal = *((i16*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case Int_32:
    {
      i32 lVal = *((i32*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case Int_64:
    {
      i64 lVal = *((i64*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case UInt_8:
    {
      u8 lVal = *((u8*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default: raiseException();
      }
      break;
    }
    case UInt_16:
    {
      u16 lVal = *((u16*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case UInt_32:
    {
      u32 lVal = *((u32*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u32) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u32) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case UInt_64:
    {
      u64 lVal = *((u64*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (i64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (i64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (i64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (i64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (i64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (u64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (u64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (u64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (u64) (lVal /  rVal))
            HANDLE_OP(BINARY_OPERATOR_REMAINDER,          (u64) (lVal %  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default : raiseException();
      }
      break;
    }
    case Float_32:
    {
      f32 lVal = *((f32*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f32) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f32) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f32) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f32) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default: raiseException();
      }
      break;
    }  
    case Float_64:
    {
      f64 lVal = *((f64*)left.value);
      switch (right.type)
      {
        case Int_8:
        {
          i8 rVal = *((i8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_16:
        {
          i16 rVal = *((i16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_32:
        {
          i32 rVal = *((i32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Int_64:
        {
          i64 rVal = *((i64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_8:
        {
          u8 rVal = *((u8*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_16:
        {
          u16 rVal = *((u16*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_32:
        {
          u32 rVal = *((u32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case UInt_64:
        {
          u64 rVal = *((u64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_32:
        {
          f32 rVal = *((f32*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        case Float_64:
        {
          f64 rVal = *((f64*)right.value);
          switch (op)
          {
            HANDLE_OP(BINARY_OPERATOR_ADDITION,           (f64) (lVal +  rVal))
            HANDLE_OP(BINARY_OPERATOR_SUBTRACTION,        (f64) (lVal -  rVal))
            HANDLE_OP(BINARY_OPERATOR_MULTIPLICATION,     (f64) (lVal *  rVal))
            HANDLE_OP(BINARY_OPERATOR_DIVISION,           (f64) (lVal /  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER,        (bool)(lVal >  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER,         (bool)(lVal <  rVal))
            HANDLE_OP(COMPARISON_OPERATOR_EQUAL,          (bool)(lVal == rVal))
            HANDLE_OP(COMPARISON_OPERATOR_GREATER_EQUAL,  (bool)(lVal >= rVal))
            HANDLE_OP(COMPARISON_OPERATOR_LESSER_EQUAL,   (bool)(lVal <= rVal))
            default: raiseException();
          }
          break;
        }
        default: raiseException();
      }
      break;
    }
    default: raiseException();
  }

  #undef HANDLE_OP

  Data retDat;
  retDat.type = resultingType;
  retDat.value = memory;

  return retDat;
}
