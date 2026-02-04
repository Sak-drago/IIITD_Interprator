#include "chunk.hpp"

int main(){
  CHUNK t_chunk;
  t_chunk._writeCHUNK(_VirtualMachine::OP_RETURN);
  t_chunk._freeCHUNK();
}
