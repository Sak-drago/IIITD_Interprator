#include "../include/garbageCollector.h"
#include "../library/include/asserts.h"
#include "../library/include/logger.h"
#include "../library/include/linearAlloc.h"
#include <unistd.h>
#include <sys/mman.h>


// - - - metadata - - - 

void*           memory    = 0;        // - - - the memory being managed
u64             capacity  = 0;        // - - - how much memory being managed
u64             size      = 0;        // - - - how much memory used
bool            started   = false;    // - - - whether we started
u64             blobCap   = 0;        // - - - how much metadata
LinearAllocator blobs;

// - - - | Functions | - - - 


// - - - create the garbage collector
void startGarbageCollector(u8 PAGES)
{
  FORGE_ASSERT_MESSAGE(!started, "[GARBAGE_COLLECTOR] : Cannot start again");

  // - - - WARN: If you are trying to understand this, do ```man mmap``` first

  // - - - get the size
  u64 pageSize  = sysconf(_SC_PAGESIZE);    
  capacity      = pageSize * (PAGES >= 1 ? PAGES : 1); 
  blobCap       = capacity / 4;     // - - - WARN: arbitrary
  
  // - - - get the memory
  createLinearAllocator(sizeof(Blob) * blobCap, 2, NULL, &blobs);
  memory        = mmap
    (
      NULL,                                                 // - - - the address
      capacity,                                             // - - - the size 
      PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,  // - - - protection : (I want to read, write and this page is mine, dont let anyone else have it)
      -1, 
      0                                                     // - - - offset      
    );

  // - - - finish
  size = 0;
  started = true;
  FORGE_LOG_INFO("[GARBAGE_COLLECTOR] : Initialized to manage %lld kb in %d pages", (capacity / 1024), (PAGES >= 1 ? PAGES : 1));
  FORGE_ASSERT_MESSAGE(memory, "[GARBAGE_COLLECTOR] : failed to get a page to manage memory. mmap failed");
}

// - - - allocate memory
void* forgeAllocate(u64 DATA_SIZE)
{
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

  // - - - get the memory to be returned
  FORGE_ASSERT_MESSAGE(DATA_SIZE + size <= capacity, "[GARBAGE COLLECTOR] : ran out of memory"); 
  void* result = (i8*)memory + size;
  size += DATA_SIZE;

  // - - - create the metadata Blob
  Blob* mrBlob    = (Blob*) linearAllocatorAllocate(&blobs, sizeof(Blob));
  mrBlob->offset  = size - DATA_SIZE;
  mrBlob->size    = DATA_SIZE;
  return result;
}

void forgeFree(void* POINTER)
{
  int x = 1;
  TODO
}


void printBlob(Blob* BLOB)
{
  char* pointer = ((char*)memory + BLOB->offset); 
  std::string value(pointer, BLOB->size);

  FORGE_LOG_INFO("Blob :-");
  FORGE_LOG_DEBUG("offset  : %lld", BLOB->offset);
  FORGE_LOG_DEBUG("size    : %lld", BLOB->size);
  FORGE_LOG_DEBUG("pointer : %p", pointer);
  FORGE_LOG_DEBUG("value   : %s\n", value.c_str());
}

void print()
{
  u64 blobCount = blobs.allocated / sizeof(Blob); 

  FORGE_LOG_TRACE("Blobs : %lld", blobCount);
  FORGE_LOG_TRACE("Capacity : %lld", capacity);
  FORGE_LOG_TRACE("Size : %lld", size);

  for (u64 i = 0; i < blobCount; ++i)
  {
    Blob* mrBlob = (Blob*)blobs.memory + i;  
    printBlob(mrBlob);
  }

  exit(1);
}
