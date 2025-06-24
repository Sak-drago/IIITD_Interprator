#include "../include/evaluator.h"
#include <cmath>

static i32 anonymousVarCount = 0;  

FORGE_INLINE bool areTypeCompatible(DataType TYPE_1, DataType TYPE_2)
{
  if (TYPE_1 == TYPE_2) return true;
  switch (TYPE_1)
  {
    // - - - numbers
    case Integer    :
    case Float      :
      switch (TYPE_2)
      {
        case Integer  :
        case Float    : return true;
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
    case BINARY_OPERATOR_REMAINDER          : return Integer;
    case BINARY_OPERATOR_POWER              : return Float;
    default :
      switch (TYPE_1)
      {
        case Bool     : return Bool; 
        case Float    : return Float;
        case Integer  : 
          switch (TYPE_2)
          {
            case Float    : return Float;
            default       : return Integer;
          }
        default       : raiseException("Not handled yet");
      }
  }
  exit(1);
}


Data evaluateBinaryNode(const Node* BINARY_NODE, const char* NAME)
{
  // - - - all these checks will disappear in the release build
  FORGE_ASSERT_MESSAGE(BINARY_NODE != NULL, "Cannot evaluate a NULL Binary Operator Node");
  FORGE_ASSERT_MESSAGE(BINARY_NODE->type == NODE_TYPE_BINARY_OPERATOR, "A Node which is not a NODE_TYPE_BINARY_OPERATOR cannot be evaluated here");

  // - - - get the memorys of the left and right Data
  Data left  = evaluate(BINARY_NODE->context.binaryContext.left,  NULL);
  Data right = evaluate(BINARY_NODE->context.binaryContext.right, NULL);
  
  if (!areTypeCompatible(left.type, right.type)) 
      raiseException("Types are nt compatible for binary operations");

  // - - - TODO: Garbage collection might want to free the memory of left and right

  BinaryOperator  op              = BINARY_NODE->context.binaryContext.opcode;
  DataType        resultingType   = getResultingDataType(left.type, right.type, op);
  std::string     name            = NAME ? std::string(NAME) : "anonymous_binary_" + std::to_string(anonymousVarCount++);
  Data*           pointer         = createVariable(name, resultingType);
  void*           memory          = pointer->memory;


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
      *((i64*)memory)  = ((i64)LVAL OP (i64)RVAL); \
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
      case Bool      :     {i8  rVal = *((i8*)right.memory);  HANDLE_OP(LTYPE, i8);  break;} \
      case Integer   :     {i64 rVal = *((i64*)right.memory); HANDLE_OP(LTYPE, i64); break;} \
      case Float     :     {f64 rVal = *((f64*)right.memory); HANDLE_OP(LTYPE, f64); break;} \
      default: raiseException("Operation on " #LTYPE " and a non-number Node is undefined");\
    } \
    break;


  switch (left.type)
  {
    case Bool:
    {
      bool lVal = *((bool*)left.memory);
      switch (right.type)
      {
        case Bool : 
          {
            bool rVal = *((bool*)right.memory);
            HANDLE_OP(bool, bool);
            break;
          }
        default : raiseException("Cannot perform any operation on a Bool and not Bool");  
      }
      break;
    }
    case Integer    : { i64 lVal  = *((i64*)left.memory);  HANDLE_BINARY_OP(i64) ; break;}
    case Float      : { f64 lVal  = *((f64*)left.memory);  HANDLE_BINARY_OP(f64) ; break;}
    default : raiseException("Operation not defined between a number and a non-number");
  }

  return *pointer;
}
