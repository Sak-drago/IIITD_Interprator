
#include <stdio.h>
#include <unistd.h> // For usleep
#include "include/garbageCollector.h"

void testAllocator()
{
    printf("Starting Garbage Collector...\n");
    startGarbageCollector(2); // Start with 2 pages

    // Allocate memory blocks
    void* p1 = forgeAllocate(256);
    //visualizeMemoryChatGPTAh(); 

    void* p2 = forgeAllocate(512);

    void* p3 = forgeAllocate(128);

    // Free middle block
    printf("\nFreeing second block...\n");
    forgeFree(p2);

    // Allocate again (should reuse the freed block)
    void* p4 = forgeAllocate(400);

    // Free all memory
    printf("\nFreeing all blocks...\n");
    forgeFree(p1);
    forgeFree(p3);
    forgeFree(p4);

    printf("\nTest completed.\n");
}

int main (int argc, char *argv[]) {
    testAllocator();
    return 0;
}
