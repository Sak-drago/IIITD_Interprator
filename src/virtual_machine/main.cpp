#include "chunk.hpp"
#include "vm.cpp"

int main(){
  _VirtualMachine _VM;
  // - - - Initialising the VM
  _VM._initVM();

  // - - - Starting the CHUNK
  CHUNK t_chunk;

  // - - - Write to the CHUNK
  size_t con_2 = t_chunk._addCONSTANT(3.4);
  size_t con = t_chunk._addCONSTANT(1.2);
  t_chunk._writeCHUNK(_VirtualMachine::OP_CONSTANT,1);
  t_chunk._writeCHUNK(con, 2);

  t_chunk._writeCHUNK(_VirtualMachine::OP_CONSTANT,1);
  t_chunk._writeCHUNK(con_2, 2);
  
  t_chunk._writeCHUNK(_VirtualMachine::OP_RETURN, 15);
  // - - - Disassemble the CHUNK
  t_chunk._disassembleCHUNK("ASHER SHOULD BE CHUNKED");

  // - - - Pass the Chunk to the VM
  _VM._interpretCHUNK(&t_chunk);

  // - - - Memory Safety
  _VM._freeVM();
  t_chunk._freeCHUNK();
}
