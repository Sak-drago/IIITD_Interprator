#include "../include/evaluator.h"
#include "../library/include/asserts.h"
#include "../include/program.h"
#include <sstream>
#include <string>

/*

std::string getDataTypeStr   (DataType DATA_TYPE)
{
  FORGE_ASSERT_MESSAGE(DATA_TYPE < DATA_TYPE_COUNT, "Not identified as a valid data type");
  switch (DATA_TYPE)
  {
    case Integer    :   return "Integer"; 
    case Float      :   return "Float"; 
    case Bool       :   return "Bool";
    case Null       :   return "Null";
    case String     :   return "String";
    case Array      :   return "Array";
    case List       :   return "List";
    case Dictionary :   return "Dictionary";
    case ERROR      :   return "ERROR";
    default         :   return "Unknown";
  }
}

std::string getDataStr        (const Data* DATA)
{
  FORGE_ASSERT_MESSAGE(DATA, "Cannot print NULL data");

  std::ostringstream stringBuilder;
  stringBuilder << "\n{\n  DataType  : "  << getDataTypeStr(DATA->type);
  stringBuilder <<  "\n  Address   : "  << std::hex << DATA->value;
  stringBuilder <<  "\n  Value     : ";

  FORGE_LOG_TRACE("%lld", *(i64*)(DATA->value));
  switch (DATA->type)
  {
    case Integer  : stringBuilder << std::to_string((i64) *static_cast<const i64*>   (DATA->value));      break;
    case Float    : stringBuilder << std::to_string((f64) *static_cast<const f64*>   (DATA->value));      break;
    case Bool     : stringBuilder << ((bool) *static_cast<const bool*>(DATA->value) ? "real" : "cap");    break;
    case Null     : stringBuilder << "Null";                                         break;
  }
  stringBuilder << "\n}";
  return stringBuilder.str();
}

i8 getDataTypeSize(const DataType DATA_TYPE) 
{
  switch(DATA_TYPE)
  {
    case Bool     : return 1;
    case Integer  :
    case Float    : return 8;
    default       : 
      raiseException("Not yet handled getting data size"); 
      return 0;
  }
}

*/
