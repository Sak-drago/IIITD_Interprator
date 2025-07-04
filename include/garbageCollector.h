#pragma once
#include "../defines.h"

typedef struct Blob
{
  u64   offset;
  u32   size;
} Blob;

FORGE_API void  startGarbageCollector   (u8    PAGES);
FORGE_API void  stopGarbageCollector();
FORGE_API void* forgeAllocate           (u32   DATA_SIZE);
FORGE_API void  forgeFree               (void* POINTER);
