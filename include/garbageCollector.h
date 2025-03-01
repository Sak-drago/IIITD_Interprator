#pragma once
#include "../defines.h"

typedef struct Blob
{
  u64 offset;
  u64   size;
} Blob;

void printBlob(Blob* BLOB);
void print();
FORGE_API void  startGarbageCollector   (u8    PAGES);
FORGE_API void* forgeAllocate           (u64   DATA_SIZE);
FORGE_API void  forgeFree               (void* POINTER);
