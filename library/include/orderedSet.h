#pragma once 
#include "../../defines.h"
#ifdef __cplusplus
extern "C" {
#endif

// - - - Allocate and Deallocate functions
typedef char* byteArray;
typedef i32    (memoryCompare)      (const void* PTR_1,   const void* PTR_2,        unsigned long SIZE);
typedef void  (memoryDeallocate)   (void*           MEM_ADDR);
typedef void* (memoryAllocate)     (unsigned long   SIZE);

// - - - structures for the AVL tree
typedef struct AVLNode
{
  byteArray          key;
  struct AVLNode*    left;
  struct AVLNode*    right;
  i32                height;
} AVLNode;

typedef struct OrderedSet
{
  AVLNode*            root;
  u64                 keySize;
  memoryCompare*      compare;
  memoryAllocate*     allocator;
  memoryDeallocate*   deallocator;
} OrderedSet;


// - - - Functions on the AVL tree - - - 

// - - - Create and Destroy
FORGE_API void createOrderedSet     (OrderedSet* SET, u64 KEY_SIZE, memoryCompare* COMPARE, memoryAllocate* MALLOC, memoryDeallocate* FREE);
FORGE_API void destroyOrderedSet    (OrderedSet* SET);

// - - - Insert, Remove, Search
FORGE_API void orderedSetInsert     (OrderedSet* SET, byteArray KEY);
FORGE_API void orderedSetRemove     (OrderedSet* SET, byteArray KEY);
FORGE_API bool orderedSetContains   (OrderedSet* SET, byteArray KEY);

#ifdef __cplusplus
}
#endif
