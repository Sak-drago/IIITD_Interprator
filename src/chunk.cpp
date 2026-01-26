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
    this->mCODE     = _GROW_VEC(u32, this->CODE, _OLD_CAPACITY, this->CAPACITY);
  }
  this->mCODE[this->mCOUNT] = _BYTE;
  this->mCOUNT++;
}
