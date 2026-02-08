#include "chunk.hpp"
#include "value.hpp"


#define I_AM_NOT_GETTING_ANYTHING_SO_DEBUG_IT_IS

void _VirtualMachine::_initVM()
{

}

void _VirtualMachine::_freeVM()
{

}

_VirtualMachine::_INTERPRET_RESULT _VirtualMachine::_interpretCHUNK(CHUNK* _CHUNK)
{
  this->mCHUNK              = _CHUNK;
  this->mINSTRUCTION_POINTER = _CHUNK->mCODE;

  #define READ_BYTE()(*this->mINSTRUCTION_POINTER++)
  #define READ_CONSTANT()(this->mCHUNK->mCONSTANTS.mVALUES[READ_BYTE()])

  for(;;)
  {
      #ifdef I_AM_NOT_GETTING_ANYTHING_SO_DEBUG_IT_IS
        this->mCHUNK->_disassembleINSTRUCTION(static_cast<int>(this->mINSTRUCTION_POINTER - this->mCHUNK->mCODE));
      #endif
    u8 _currentINSTRUCTION = READ_BYTE();
    switch (_currentINSTRUCTION) 
    {
      case _VirtualMachine::OP_CONSTANT:
        {
          Value _CONSTANT = READ_CONSTANT();
          this->mCHUNK->mCONSTANTS._printVALUE(_CONSTANT);
          std::cout << '\n';
          break;
        }
      case _VirtualMachine::OP_RETURN:
        {
          return _VirtualMachine::_INTERPRET_RESULT::INTERPRET_OK;
        }
    }
  }
  
  // - - - Just making sure that we are not using the macro OUTSIDE of this function.
  #undef READ_BYTE
  #undef READ_CONSTANT
}
