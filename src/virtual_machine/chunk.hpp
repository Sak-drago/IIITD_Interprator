#ifndef iiit_CHUNK_HPP
#define iiit_CHUNK_HPP

#include "common.hpp"
#include "value.hpp"

#define TOTAL_STACK_SPACE 512

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
 // - - - Separate area for VM stuff
 typedef enum{
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
  INTERPRET_COUNT,
 } _INTERPRET_RESULT;

 void _initVM();
 void _freeVM();
 
 void _resetSTACK();
public:
 typedef enum{
  OP_CONSTANT,
  OP_RETURN,
  OP_COUNT,
 } _OPCODE;

// - - - Defining the chunk structure here. Need to make it dynamic!
 struct _CHUNK{
  size_t mCOUNT;   // - - -Using size_t instead of u32 or something because I don't know platforms my guy
  size_t mCAPACITY;
  u8* mCODE;
  i32* mLINES;
  ValueARRAY mCONSTANTS;

  // - - - Functions to manage _CHUNK
  _CHUNK();
  void _writeCHUNK(u8 _BYTE, u8 _LINE);
  void _freeCHUNK();
  void _resetCHUNK();
  // - - -  Functions to disassemble/debug the chunk
  void _disassembleCHUNK(const char* _NAME);
  size_t _disassembleINSTRUCTION(size_t _OFFSET);

  // - - - Function(s) to manipulate constants
  i8 _addCONSTANT(Value _VALUE);
 };

 struct _INSTRUCTION_PROCESSORS{
  static size_t _simpleINSTRUCTION(const char* _NAME, size_t _OFFSET);
  static size_t _constantINSTRUCTION(const char* _NAME, _CHUNK* _CHUNK ,size_t _OFFSET);
 };
 private:
 _VirtualMachine::_CHUNK* mCHUNK;
 u8* mINSTRUCTION_POINTER;

 public:
 _CHUNK* _getvmCHUNK(){return mCHUNK;}
 void   _setvmCHUNK(_CHUNK* _otherCHUNK){mCHUNK = _otherCHUNK;}

 u8* _getIP(){return mINSTRUCTION_POINTER;}
 void _setIP(u8* _CODE){mINSTRUCTION_POINTER = _CODE;}
 
 //NOTE: The _STACK_TOP pointer points towards +1 from the top. Not the exact top element.
 // Default stack size is 512.
 
 // - - - Stack area 
 Value mSTACK[TOTAL_STACK_SPACE];
 Value* mSTACK_TOP = 0;
 void _PUSH_STACK(Value _VALUE);
 Value _POP_STACK();

 _INTERPRET_RESULT _interpretCHUNK(_CHUNK* _CHUNK);
};
using CHUNK = struct _VirtualMachine::_CHUNK;
using INSTRUCTION = struct _VirtualMachine::_INSTRUCTION_PROCESSORS;
#endif
