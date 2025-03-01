#pragma once
#include "../../defines.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct objectPool
{
  u64   objectSize;
  u64   capacity;
  u64   freeListOffset;
  f32   resizeFactor;
  void* memory;
} ObjectPool;

FORGE_API void  createObjectPool(u64 TOTAL_CAPACITY, u64 OBJECT_SIZE, f32 RESIZE_FACTOR, void* MEMORY, ObjectPool* POOL);

FORGE_API void* takeObject(ObjectPool* POOL);

FORGE_API void  returnObject(ObjectPool* POOL, void* OBJECT);

FORGE_API void  destroyObjectPool(ObjectPool* POOL);

FORGE_API void visualizeObjectPool(ObjectPool* POOL);


#ifdef __cplusplus
}
#endif
