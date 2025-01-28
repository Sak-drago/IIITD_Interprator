#include "../include/data.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include <sstream>


std::string getDataTypeStr   (const DataType* DATA_TYPE)
{
  FORGE_ASSERT_MESSAGE(DATA_TYPE != NULL, "Cannot print NULL");
  switch (*DATA_TYPE)
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
    case Tuple      :   return "Tuple";
    case ERROR      :   return "ERROR";
    default         :   return "Unknown";
  }
}


std::string getDataStr        (const Data* DATA)
{
  FORGE_ASSERT_MESSAGE(DATA, "Cannot print NULL data");

  std::ostringstream stringBuilder;
  stringBuilder << "\n{\n  DataType  : "  << getDataTypeStr(&DATA->type);
  stringBuilder <<  "\n  Address   : "  << std::hex << DATA->value;
  stringBuilder <<  "\n  Value     : ";

  switch (DATA->type)
  {
    case Int_8    : stringBuilder << (i64) *static_cast<const i8*>  (DATA->value); break;
    case UInt_8   : stringBuilder << (u64) *static_cast<const u8*>  (DATA->value); break;
    case Int_16   : stringBuilder << (i64) *static_cast<const i16*> (DATA->value); break;
    case UInt_16  : stringBuilder << (u64) *static_cast<const u16*> (DATA->value); break;
    case Int_32   : stringBuilder << (i64) *static_cast<const i32*> (DATA->value); break;
    case UInt_32  : stringBuilder << (u64) *static_cast<const u32*> (DATA->value); break;
    case Int_64   : stringBuilder << (i64) *static_cast<const i64*> (DATA->value); break;
    case UInt_64  : stringBuilder << (u64) *static_cast<const u64*> (DATA->value); break;
    case Float_32 : stringBuilder << (f32) *static_cast<const f32*> (DATA->value); break;
    case Float_64 : stringBuilder << (f64) *static_cast<const f64*> (DATA->value); break;
    case Bool     : stringBuilder << (bool) *static_cast<const bool*>(DATA->value); break;
    case Null     : stringBuilder << "Null";                                 break;
  }
  stringBuilder << "\n}";
  return stringBuilder.str();
}
