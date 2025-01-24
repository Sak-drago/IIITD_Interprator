#pragma once 
#include "parser.h"

typedef enum ObjectType
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
} ObjectType;

typedef struct Object 
{
  ObjectType type;
  void*      value;
} Object;

FORGE_API Object evaluate(Node* NODE);
