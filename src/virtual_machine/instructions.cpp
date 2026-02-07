#include "chunk.hpp"
#include "utils/logger.hpp"
#include "value.hpp"
#include <iomanip>

size_t INSTRUCTION::_simpleINSTRUCTION(const char *_NAME, size_t _OFFSET)
{
  LOG_INFO(_NAME);
  std::cout << '\n';
  return _OFFSET+1;
}

size_t INSTRUCTION::_constantINSTRUCTION(const char *_NAME, _CHUNK *_CHUNK, size_t _OFFSET)
{
  u8 _CONSTANT = _CHUNK->mCODE[_OFFSET+1];
  LOG_INFO(_NAME);
  std::cout << LOG_COLOR_INFO << std::setw(16) << " " << _CONSTANT << " '" << LOG_COLOR_RESET;
  _CHUNK->mCONSTANTS._printVALUE(_CHUNK->mCONSTANTS.mVALUES[_CONSTANT]);
  std::cout << '\n';
  return _OFFSET+2;
}
