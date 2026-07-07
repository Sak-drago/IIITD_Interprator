#include "chunk.hpp"
#include "value.hpp"
#define I_AM_NOT_GETTING_ANYTHING_SO_DEBUG_IT_IS


void _VirtualMachine::_PUSH_STACK(Value _VALUE)
{
  *this->mSTACK_TOP = _VALUE;
  this->mSTACK_TOP++;
}

Value _VirtualMachine::_POP_STACK()
{
  this->mSTACK_TOP--;
  return *this->mSTACK_TOP;
}

void _VirtualMachine::_resetSTACK()
{
  this->mSTACK_TOP = this->mSTACK;
}


void _VirtualMachine::_initVM()
{
  this->_resetSTACK();
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
  // - - - Pop 2, apply op, push 1. Value is just a double for now so no type checks needed yet.
  #define BINARY_OP(_OP) \
    do { \
      Value _B = _POP_STACK(); \
      Value _A = _POP_STACK(); \
      _PUSH_STACK(_A _OP _B); \
    } while(false)

  for(;;)
  {
      #ifdef I_AM_NOT_GETTING_ANYTHING_SO_DEBUG_IT_IS
        std::cout << "======== STACK TRACE BACK ========" << '\n';
        for(Value* _SLOT = this->mSTACK; _SLOT < this->mSTACK_TOP; _SLOT++)
        {
          std::cout << "[";
          this->mCHUNK->mCONSTANTS._printVALUE(*_SLOT);
          std::cout << "]";
        }
        std::cout << std::endl;
        this->mCHUNK->_disassembleINSTRUCTION(static_cast<int>(this->mINSTRUCTION_POINTER - this->mCHUNK->mCODE));
      #endif
    u8 _currentINSTRUCTION = READ_BYTE();
    switch (_currentINSTRUCTION) 
    {
      case _VirtualMachine::OP_CONSTANT:
        {
          Value _CONSTANT = READ_CONSTANT();
          _PUSH_STACK(_CONSTANT);
          break;
        }
      case _VirtualMachine::OP_ADD:      BINARY_OP(+); break;
      case _VirtualMachine::OP_SUBTRACT: BINARY_OP(-); break;
      case _VirtualMachine::OP_MULTIPLY: BINARY_OP(*); break;
      case _VirtualMachine::OP_DIVIDE:   BINARY_OP(/); break;
      case _VirtualMachine::OP_NEGATE:
        {
          _PUSH_STACK(-_POP_STACK());
          break;
        }
      case _VirtualMachine::OP_RETURN:
        {
          this->mCHUNK->mCONSTANTS._printVALUE(_POP_STACK());
          std::cout << " <- OP_RETURN\n";
          return _VirtualMachine::_INTERPRET_RESULT::INTERPRET_OK;
        }
    }
  }

  // - - - Just making sure that we are not using the macro OUTSIDE of this function.
  #undef READ_BYTE
  #undef READ_CONSTANT
  #undef BINARY_OP
}
