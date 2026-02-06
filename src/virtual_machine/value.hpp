#ifndef iiit_VALUE_HPP 
#define iiit_VALUE_HPP

#include "common.hpp"
// - - - To manipulate allowed type values later.
using Value = double;

// - - - idea is to create a const pool (inspired from JVM)
struct _valueARRAY{
  size_t mCAPACITY;
  size_t mCOUNT;
  Value* mVALUES;

  // - - - supporting functions for const object pool
  _valueARRAY();
  void _writeValueARRAY(Value _VALUE);
  void _freeValueARRAY();
  void _resetValueARRAY();
  void _printVALUE(Value _VALUE);
};

using ValueARRAY = _valueARRAY;
#endif 
