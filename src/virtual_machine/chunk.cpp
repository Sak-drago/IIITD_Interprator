#include "chunk.hpp"

// - - - init CHUNK
CHUNK::_CHUNK(){
  mCOUNT     = 0;
  mCAPACITY  = 0;
  mCODE      = nullptr;
}

void CHUNK::_writeCHUNK(u8 _BYTE){
  if(this->mCAPACITY < this->mCOUNT + 1){
    size_t _OLD_CAPACITY = this->mCAPACITY;
    this->mCAPACITY = _GROW_CAPACITY(_OLD_CAPACITY);
    this->mCODE     = _GROW_VEC<u8>(this->mCODE, _OLD_CAPACITY, this->mCAPACITY);
  }
  this->mCODE[this->mCOUNT] = _BYTE;
  this->mCOUNT++;
}

void CHUNK::_freeCHUNK(){
  _FREE_VEC<u8>(this->mCODE, this->mCAPACITY);
  this->mCAPACITY = 0;
  this->mCOUNT    = 0;
  this->mCODE     = nullptr;
}


