#include "value.hpp"
#include "chunk.hpp"

ValueARRAY::_valueARRAY()
{
  this->mVALUES     = nullptr;
  this->mCAPACITY   = 0;
  this->mCOUNT      = 0;
}

void ValueARRAY::_resetValueARRAY()
{
  this->mVALUES     = nullptr;
  this->mCAPACITY   = 0;
  this->mCOUNT      = 0;
}

void ValueARRAY::_writeValueARRAY(Value _VALUE)
{
  if(this->mCAPACITY < this->mCOUNT+1)
  {
    size_t mOLDCAPACITY = this->mCAPACITY;
    this->mCAPACITY     = _GROW_CAPACITY(mOLDCAPACITY);
    this->mVALUES       = _GROW_VEC<Value>(this->mVALUES, mOLDCAPACITY, this->mCAPACITY);
  }
  
  this->mVALUES[this->mCOUNT] = _VALUE;
  this->mCOUNT++;
}

void ValueARRAY::_freeValueARRAY()
{
  _FREE_VEC<Value>(this->mVALUES, this->mCAPACITY);
  this->_resetValueARRAY();
}

void ValueARRAY::_printVALUE(Value _VALUE)
{
  std::cout << _VALUE << "'";
}
