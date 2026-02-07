#include "chunk.hpp"

// - - - init CHUNK
CHUNK::_CHUNK(){
  mCOUNT     = 0;
  mCAPACITY  = 0;
  mCODE      = nullptr;
  mLINES     = nullptr;
}

void CHUNK::_resetCHUNK()
{
  this->mCAPACITY = 0;
  this->mCOUNT    = 0;
  this->mLINES    = nullptr;
  this->mCODE     = nullptr;
}

void CHUNK::_writeCHUNK(u8 _BYTE, u8 _LINE)
{
  if(this->mCAPACITY < this->mCOUNT + 1){
    size_t _OLD_CAPACITY = this->mCAPACITY;
    this->mCAPACITY = _GROW_CAPACITY(_OLD_CAPACITY);
    this->mCODE     = _GROW_VEC<u8>(this->mCODE, _OLD_CAPACITY, this->mCAPACITY);
    this->mLINES    = _GROW_VEC<i32>(this->mLINES, _OLD_CAPACITY, this->mCAPACITY);
  }
  this->mCODE[this->mCOUNT]  = _BYTE;
  this->mLINES[this->mCOUNT] = _LINE;
  this->mCOUNT++;
}

void CHUNK::_freeCHUNK()
{
  _FREE_VEC<u8>(this->mCODE, this->mCAPACITY);
  _FREE_VEC<i32>(this->mLINES, this->mCAPACITY);
  this->mCONSTANTS._freeValueARRAY();
  this->_resetCHUNK();
}

// - - - Function(s) to manipulate Constants implemented
i8 CHUNK::_addCONSTANT(Value _VALUE)
{
  this->mCONSTANTS._writeValueARRAY(_VALUE);
  return ((this->mCONSTANTS.mCOUNT) - 1);
}

