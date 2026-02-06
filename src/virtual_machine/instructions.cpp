#include "chunk.hpp"
#include "utils/logger.hpp"
#include "value.hpp"
#include <iomanip>

size_t INSTRUCTION::_simpleINSTRUCTION(const char *_NAME, size_t _OFFSET)
{
  LOG_TRACE(_NAME);
  return _OFFSET+1;
}

size_t INSTRUCTION::_constantINSTRUCTION(const char *_NAME, _CHUNK *_CHUNK, size_t _OFFSET)
{
  u8 _CONSTANT = _CHUNK->mCODE[_OFFSET+1];
  std::cout << LOG_COLOR_INFO << std::left << std::setw(16) << _NAME << "  " << std::right << std::setw(4) << _CONSTANT << " '" << LOG_COLOR_RESET;
  _CHUNK->mCONSTANTS._printVALUE(_CHUNK->mCONSTANTS.mVALUES[_CONSTANT]);
  std::cout << '\n';
  return _OFFSET+2;
}
