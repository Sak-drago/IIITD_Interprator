#include "chunk.hpp"
#include <iomanip>
#include <utils/logger.hpp>

size_t CHUNK::_disassembleINSTRUCTION(size_t _OFFSET)
{
  std::cout << "\033[32m" << std::hex << std::setw(4) << std::setfill('0') << _OFFSET << " " << std::dec << std::setfill(' ') << " " << "\033[0m";
  if(_OFFSET > 0 && this->mLINES[_OFFSET] == this->mLINES[_OFFSET - 1])
  {
    std::cout << "    |";
  }
  else
  {
    std::cout << std::setw(4) << this->mLINES[_OFFSET] << " ";  
  }

  u8 _INSTRUCTION = this->mCODE[_OFFSET];
  switch(_INSTRUCTION)
  {
    case _VirtualMachine::OP_CONSTANT:
      return INSTRUCTION::_constantINSTRUCTION("OP_CONSTANT", this, _OFFSET);
    case _VirtualMachine::OP_RETURN:
      return INSTRUCTION::_simpleINSTRUCTION("OP_RETURN", _OFFSET);
    default:
      std::cout << "NOT A VALID INSTRUCTION: "<< std::hex << std::setw(4) << std::setfill('0') << _OFFSET << " " << std::dec;
      LOG_FATAL("UNKNOWN INSTRUCTION");
      return _OFFSET+1;
  }
}

void CHUNK::_disassembleCHUNK(const char* _NAME)
{
  LOG_INFO("=== CURRENT CHUNK: ");
  std::cout << "\033[32m" <<  _NAME << " ==="  << "\033[0m" << '\n';
  for(size_t _OFFSET = 0; _OFFSET < this->mCOUNT;)
  {
    _OFFSET = _disassembleINSTRUCTION(_OFFSET);
  }
}
