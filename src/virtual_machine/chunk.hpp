#ifndef iiit_CHUNK_HPP
#define iiit_CHUNK_HPP

#include "common.hpp"
// - - - HELPER FUNCTIONS:
// - - - NOTE: 4 cases, none-> non-zero, non-zero -> none, non-zero -> smaller, non-zero -> bigger 
static inline void* reallocate(void* mPOINTER, size_t mOLDCOUNT, size_t mNEWCOUNT){
 if(mNEWCOUNT == 0)
 {
  free(mPOINTER);
  return nullptr;
 }
 
 void* _result = realloc(mPOINTER, mNEWCOUNT);
 // - - - Incase we run out of memory
 if(_result == nullptr){std::cerr << "\033[32m NOT ENOUGH MEMORY AT REALLOC for:" << mPOINTER << "\033[0m"; exit(1);}
 return _result;
}

// - - - Dynamic Array Helpers
// NOTE: I would avoid 1.5 since I am NOT WORKING WITH FLOATS
static inline int _GROW_CAPACITY(size_t mCAPACITY){return ((mCAPACITY) < 8 ? 8 : (mCAPACITY) * 2);}

template  <typename T>
static inline T* _GROW_VEC(T* mPOINTER, size_t mOLDCOUNT, size_t mNEWCOUNT){ return static_cast<T*>(reallocate(mPOINTER, sizeof(T)* mOLDCOUNT, sizeof(T)* mNEWCOUNT));}

template <typename T>
static inline T* _FREE_VEC(T* mPOINTER, size_t mOLDCOUNT){return static_cast<T*>(reallocate(mPOINTER, sizeof(T*)*mOLDCOUNT, 0));}

// - - - Defining the byte structure here.
class _VirtualMachine{
public:
 typedef enum{
  OP_RETURN,
 } OpCode;

// - - - Defining the chunk structure here. Need to make it dynamic!
 struct _CHUNK{
  size_t mCOUNT;   // - - -Using size_t instead of u32 or something because I don't know platforms my guy
  size_t mCAPACITY;
  u8* mCODE;

  // - - - Functions to manage _CHUNK
  _CHUNK();
  void _writeCHUNK(u8 _BYTE);
  void _freeCHUNK();

  // - - -  Functions to disassemble/debug the chunk
  void _disassembleCHUNK(const char* _NAME);
  size_t _disassembleINSTRUCTION(size_t _OFFSET);
 };

 struct _INSTRUCTION_PROCESSORS{
  static size_t _simpleINSTRUCTION(const char* _NAME, size_t _OFFSET);
 };
};
using CHUNK = struct _VirtualMachine::_CHUNK;
using INSTRUCTION = struct _VirtualMachine::_INSTRUCTION_PROCESSORS;
#endif
