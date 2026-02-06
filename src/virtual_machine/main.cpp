#include "chunk.hpp"

int main(){
  // - - - Starting the CHUNK
  CHUNK t_chunk;
  // - - - Write to the CHUNK
  t_chunk._writeCHUNK(_VirtualMachine::OP_RETURN);
  // - - - Disassemble the CHUNK
  t_chunk._disassembleCHUNK("ASHER SHOULD BE CHUNKED");
  // - - - Memory Safety
  t_chunk._freeCHUNK();
}
