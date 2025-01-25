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
  DataType type;
  void*    value;
} Data;

FORGE_API Data evaluate(Node* NODE);
