#include "../include/evaluator.h"
#include "../library/include/asserts.h"
#include <sstream>
#include <string>


std::string getDataTypeStr   (const DataType DATA_TYPE)
{
  FORGE_ASSERT_MESSAGE(DATA_TYPE < DATA_TYPE_COUNT, "Not identified as a valid data type");
  switch (DATA_TYPE)
  {
    case Int_8      :   return "Int_8";
    case Int_16     :   return "Int_16";
    case Int_32     :   return "Int_32";
    case Int_64     :   return "Int_64";
    case Float_32   :   return "Float_32";
    case Float_64   :   return "Float_64";
    case UInt_8     :   return "UInt_8";
    case UInt_16    :   return "UInt_16";
    case UInt_32    :   return "UInt_32";
    case UInt_64    :   return "UInt_64";
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

  switch (DATA->type)
  {
    case Int_8    : stringBuilder << std::to_string((i64) *static_cast<const i8*>    (DATA->value));      break;
    case UInt_8   : stringBuilder << std::to_string((u64) *static_cast<const u8*>    (DATA->value));      break;
    case Int_16   : stringBuilder << std::to_string((i64) *static_cast<const i16*>   (DATA->value));      break;
    case UInt_16  : stringBuilder << std::to_string((u64) *static_cast<const u16*>   (DATA->value));      break;
    case Int_32   : stringBuilder << std::to_string((i64) *static_cast<const i32*>   (DATA->value));      break;
    case UInt_32  : stringBuilder << std::to_string((u64) *static_cast<const u32*>   (DATA->value));      break;
    case Int_64   : stringBuilder << std::to_string((i64) *static_cast<const i64*>   (DATA->value));      break;
    case UInt_64  : stringBuilder << std::to_string((u64) *static_cast<const u64*>   (DATA->value));      break;
    case Float_32 : stringBuilder << std::to_string((f32) *static_cast<const f32*>   (DATA->value));      break;
    case Float_64 : stringBuilder << std::to_string((f64) *static_cast<const f64*>   (DATA->value));      break;
    case Bool     : stringBuilder << ((bool) *static_cast<const bool*>(DATA->value) ? "Real" : "Cap");    break;
    case Null     : stringBuilder << "Null";                                         break;
  }
  stringBuilder << "\n}";
  return stringBuilder.str();
}

i8 getDataTypeSize(const DataType DATA_TYPE) 
{
  switch(DATA_TYPE)
  {
    case Bool     :
    case Int_8    :
    case UInt_8   : return 1;

    case Int_16   : 
    case UInt_16  : return 2;

    case Float_32 :
    case Int_32   : 
    case UInt_32  : return 4;

    case Float_64 :
    case Int_64   : 
    case UInt_64  : return 8;

    default       : 
      raiseException("Not yet handled getting data size"); 
      return 0;
  }
}
