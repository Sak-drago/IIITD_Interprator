#pragma once
#include "../defines.h"

typedef struct Blob
{
  u64   offset;
  u32   size;
  u32   refCount;
} Blob;

FORGE_API void  visualizeMemoryChatGPTAh();
FORGE_API void  startGarbageCollector   (u8    PAGES);
FORGE_API void* forgeAllocate           (u32   DATA_SIZE);
FORGE_API void  forgeFree               (void* POINTER);
