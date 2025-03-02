#include "../include/garbageCollector.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../library/include/objectPool.h"
#include "../library/include/orderedSet.h"
#include <unistd.h>
#include <sys/mman.h>


// - - -  metadata  - - - 

void*           memory    = 0;        // - - - the memory being managed
u64             capacity  = 0;        // - - - how much memory being managed
u64             size      = 0;        // - - - how much memory used
bool            started   = false;    // - - - whether we started
u64             blobCap   = 0;        // - - - how much metadata
ObjectPool      blobs;                // - - - the blobs
ObjectPool      setsNodes;            // - - - AVL tree Nodes
OrderedSet      slaveBlobs;           // - - - AVL tree for allocated (balanced on offset)
OrderedSet      freeBlobs;            // - - - AVL tree for free (balanced on size and then offset)


// - - - Helper Functionality - - - 

i32 compareSlaveBlobs(const void* SLAVE_1, const void* SLAVE_2, size_t SIZE)
{
  FORGE_ASSERT_MESSAGE(SLAVE_1, "[GARBAGE COLLECTOR] : Lost track of one allocation");
  FORGE_ASSERT_MESSAGE(SLAVE_2, "[GARBAGE COLLECTOR] : Lost track of one allocation");

  const Blob* blobA = (const Blob*) SLAVE_1;
  const Blob* blobB = (const Blob*) SLAVE_2;

  return (i64)blobA->offset - (i64)blobB->offset;
}

i32 compareFreeBlobs(const void* BLOB_1, const void* BLOB_2, size_t SIZE)
{
  FORGE_ASSERT_MESSAGE(BLOB_1, "[GARBAGE COLLECTOR] : Lost track of one free fragment");
  FORGE_ASSERT_MESSAGE(BLOB_2, "[GARBAGE COLLECTOR] : Lost track of one free fragment");

  const Blob* blobA = (const Blob*)BLOB_1;
  const Blob* blobB = (const Blob*)BLOB_2;
  
  if (blobA->size < blobB->size) return -1;
  if (blobA->size > blobB->size) return 1;
  
  return (blobA->offset < blobB->offset) ? -1 : (blobA->offset > blobB->offset);
}

void* mallocSetNode(unsigned long BYTES) { return takeObject(&setsNodes); }
void freeSetNode(void* PTR) { returnObject(&setsNodes, PTR); }

void printBlob(byteArray a)
{
  Blob* BLOB= (Blob*) a;
  printf("[ offset : %llu, size : %d, refcount : %d ] \n", BLOB->offset, BLOB->size, BLOB->refCount);
}

void printTree(OrderedSet* TREE)
{
  orderedSetTraverse(TREE, printBlob, TRAVERSAL_IN_ORDER);
}


// - - - | Functions | - - - 


// - - - create the garbage collector
void startGarbageCollector(u8 PAGES)
{
  FORGE_ASSERT_MESSAGE(!started, "[GARBAGE_COLLECTOR] : Cannot start again");
  FORGE_ASSERT_MESSAGE(PAGES,    "[GARBAGE_COLLECTOR] : Must be responsible for atleast 1 page of memory");

  // - - - WARN: If you are trying to understand this, do ```man mmap``` first

  // - - - get the size
  u64 pageSize  = sysconf(_SC_PAGESIZE);    
  capacity      = pageSize * PAGES;
  blobCap       = capacity / 4;     // - - - WARN: arbitrary, @Sakshat, needs to provide count of assignment statements
   
  // - - - get the memory
  memory        = mmap
    (
      NULL,                                                 // - - - the address
      capacity,                                             // - - - the size 
      PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,  // - - - protection : (I want to read, write and this page is mine, dont let anyone else have it)
      -1, 
      0                                                     // - - - offset      
    );
  
  // - - - make room for blobs and setNodes
  createObjectPool(blobCap, sizeof(Blob), NULL, &blobs);
  createObjectPool(blobCap, sizeof(AVLNode), NULL, &setsNodes);

  // - - - make the two ordered sets 
  createOrderedSet(&slaveBlobs, sizeof(Blob), compareSlaveBlobs, mallocSetNode, freeSetNode);
  createOrderedSet(&slaveBlobs, sizeof(Blob), compareFreeBlobs,  mallocSetNode, freeSetNode);

  // - - - finish
  size    = 0;
  started = true;
  FORGE_LOG_INFO("[GARBAGE_COLLECTOR] : Initialized to manage %lld kb in %d pages handling at most %lld variables", (capacity / 1024), (PAGES >= 1 ? PAGES : 1), blobCap);
  FORGE_ASSERT_MESSAGE(memory, "[GARBAGE_COLLECTOR] : failed to get a page to manage memory. mmap failed");
}

// - - - allocate memory
void* forgeAllocate(u32 DATA_SIZE)
{
  FORGE_LOG_DEBUG("here are the slaves");
  printTree(&slaveBlobs);
  // - - - start the garbage collector if not already
  if (!started) 
  {
    FORGE_LOG_WARNING("[GARBAGE_COLLECTOR] :Starting Garbage Collection with 1 Page only. If you want more memory to be managed, call 'startGarbageCollector()' explicitly");
    startGarbageCollector(1);
  }

  // - - - dont allocate 0 bytes
  if (DATA_SIZE == 0)
  {
    FORGE_LOG_ERROR("[GARBAGE_COLLECTOR]: Tried to allocate 0 bytes");
    return NULL;
  }

  // - - - try to find memory from the free blobs 
  Blob query = 
    {
      .offset   = 0,
      .size     = DATA_SIZE, 
      .refCount = 0
    };
  Blob* matchBlob = (Blob*) orderedSetFindSmallestAtleast(&freeBlobs, (byteArray)&query);

  if (matchBlob)
  {
    FORGE_LOG_TRACE("[GARBAGE_COLLECTOR] : Found a free blob to allocate memory (%lld bytes) from", DATA_SIZE);
    if (matchBlob->size > DATA_SIZE)
    {
      FORGE_LOG_TRACE("[GARBAGE_COLLECTOR] : Free Blob too big (%lld bytes), splitting into two smaller blobs", matchBlob->size);
      Blob* remaining     = (Blob*) takeObject(&blobs);
      remaining->offset   = matchBlob->offset + DATA_SIZE;
      remaining->size     = matchBlob->size   - DATA_SIZE;
      remaining->refCount = 0;
      orderedSetInsert(&freeBlobs, (byteArray)remaining);
    }

    matchBlob->size     = DATA_SIZE;
    matchBlob->refCount = 1;
    orderedSetInsert(&slaveBlobs, (byteArray)matchBlob);

    // - - - get the memory to be returned
    return (void*) ((u8*)memory + matchBlob->offset);
  }

  // - - - No suitable free blob
  FORGE_LOG_WARNING("[GARBAGE_COLLECTOR] : Couldnt find any suitable free Blob, making one for %lld bytes", DATA_SIZE);

  // - - - get the memory to be returned
  FORGE_ASSERT_MESSAGE(DATA_SIZE + size <= capacity, "[GARBAGE COLLECTOR] : ran out of memory"); 
  void* result = (i8*)memory + size;
  size += DATA_SIZE;

  // - - - create the metadata Blob
  Blob* mrBlob = (Blob*) takeObject(&blobs);
  mrBlob->offset = size - DATA_SIZE;
  mrBlob->size = DATA_SIZE;
  mrBlob->refCount = 1;
  FORGE_LOG_DEBUG("Here is mr bkob");
  printBlob((byteArray)mrBlob);
  orderedSetInsert(&slaveBlobs, (byteArray)mrBlob);

  return result;
}

void forgeFree(void* POINTER)
{
  FORGE_LOG_INFO("Printing the slaves");
  printTree(&slaveBlobs);
  FORGE_LOG_INFO("Slves ^\n")
  FORGE_ASSERT_MESSAGE(started, "[GARBAGE_COLLECTOR] : Cannot free anything before an allocation has been made : i.e : garbage collector has been started");
  FORGE_ASSERT_MESSAGE(POINTER, "[GARBAGE COLLECTOR] : Cannot free a NULL pointer");

  // - - - make offset from pointer and ensure that it was allocated
  u64 offset    = (u8*) POINTER - (u8*) memory; 
  Blob temp     = {offset, 0, 0};
  Blob* actual  = (Blob*) orderedSetFindSmallestAtleast(&slaveBlobs, (byteArray)&temp);
  FORGE_ASSERT_MESSAGE(actual,                   "[GARBAGE_COLLECTOR] : Tried to free memory that doesnt exist");
  printBlob((byteArray)&temp);
  printBlob((byteArray)actual);
  FORGE_ASSERT_MESSAGE(actual->offset == offset, "[GARBAGE_COLLECTOR] : Tried to free memory that was never allocated");

  // - - - delete it from the slaves and add a free blob with the same offset and size to the free set
  orderedSetRemove(&slaveBlobs, (byteArray) &temp);
  Blob newBlob  = {offset, actual->size, 0}; 
  orderedSetInsert(&freeBlobs,  (byteArray) &newBlob);

  // - - - perform merge with successor and predecessor
  while (true)
  {
    Blob* successor   = (Blob*) orderedSetSuccessor  (&freeBlobs, (byteArray)&newBlob);
    Blob* predecessor = (Blob*) orderedSetPredecessor(&freeBlobs, (byteArray)&newBlob);

    if (successor && newBlob.offset + newBlob.size == successor->offset)
    {  
      orderedSetRemove(&freeBlobs, (byteArray) &newBlob);
      orderedSetRemove(&freeBlobs, (byteArray) successor);

      newBlob = 
        { 
          .offset   = newBlob.offset, 
          .size     = newBlob.size + successor->size, 
          .refCount = 0
        };
      orderedSetInsert(&freeBlobs, (byteArray)&newBlob);
      continue;
    }

    else if (predecessor && newBlob.offset == predecessor->offset + predecessor->size)
    {
      orderedSetRemove(&slaveBlobs, (byteArray) &newBlob);
      orderedSetRemove(&slaveBlobs, (byteArray) predecessor);

      newBlob = 
        {
          .offset   = predecessor->offset, 
          .size     = newBlob.size + predecessor->size, 
          .refCount = 0
        };
      orderedSetInsert(&freeBlobs, (byteArray)&newBlob);
      continue;
    }
    break;
  }  
}

