#include "chunk.hpp"
#include "utils/logger.hpp"


size_t INSTRUCTION::_simpleINSTRUCTION(const char *_NAME, size_t _OFFSET)
{
  LOG_TRACE(_NAME);
  return _OFFSET+1;
}
