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
        case Float_64 :
          return true;
        default : return false; 
      }

    default : return false;
  }
  return false;
}
