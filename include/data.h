#pragma once 
#include "parser.h"

typedef enum DataType
{
  Int_8,
  Int_16,
  Int_32,
  Int_64,

  Float_32,
  Float_64,

  UInt_8,
  UInt_16,
  UInt_32,
  UInt_64,

  Bool,
  Null,
  String,

  Array,
  List,
  Dictionary,
  Tuple,

  ERROR,
  ObjectTypeCount
} DataType;

typedef struct Data
{
  const DataType type;
  const void*    value;
} Data;


FORGE_API Data        evaluate          (Node* NODE);

FORGE_API std::string getDataTypeStr    (const DataType* DATA_TYPE);

FORGE_API std::string getDataStr        (const Data* DATA);
