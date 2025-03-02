/*
#include <stdio.h>
#include <stdlib.h>
#include "library/include/orderedSet.h"
#include "library/include/objectPool.h"

// Structure representing a memory blob
typedef struct {
    size_t size;
    size_t offset;
} Blob;

ObjectPool nodesSet;

// Comparison function: sorts by size first, then by offset
int compareBlobs(const void* a, const void* b, size_t keySize) {
    const Blob* blobA = (const Blob*)a;
    const Blob* blobB = (const Blob*)b;
    
    if (blobA->size < blobB->size) return -1;
    if (blobA->size > blobB->size) return 1;
    
    return (blobA->offset < blobB->offset) ? -1 : (blobA->offset > blobB->offset);
}

// Print function to display blobs in the tree
void printBlob(byteArray key) {
    Blob* blob = (Blob*)key;
    printf("[Size: %zu, Offset: %zu] ", blob->size, blob->offset);
}

void* Malloc(unsigned long BYTES) {
    return takeObject(&nodesSet);
}

void Free(void* PTR) {
    returnObject(&nodesSet, PTR);
}

// Function to find and merge adjacent blobs
void mergeAdjacentBlobs(OrderedSet* blobSet) {
    OrderedSetIterator it; 
    createOrderedSetIter(blobSet, &it);
    Blob* current = (Blob*)orderedSetIterNext(blobSet, &it);

    while (current) {
        Blob nextBlobQuery = {current->size, current->offset + current->size}; 
        Blob* nextBlob = (Blob*)orderedSetFindSmallestAtleast(blobSet, (byteArray)&nextBlobQuery);

        if (nextBlob && nextBlob->offset == current->offset + current->size) {
            printf("Merging blobs: [Size: %zu, Offset: %zu] + [Size: %zu, Offset: %zu]\n",
                   current->size, current->offset, nextBlob->size, nextBlob->offset);

            Blob merged = {current->size + nextBlob->size, current->offset};
            orderedSetRemove(blobSet, (byteArray)current);
            orderedSetRemove(blobSet, (byteArray)nextBlob);
            orderedSetInsert(blobSet, (byteArray)&merged);

            createOrderedSetIter(blobSet, &it);
            current = (Blob*)orderedSetIterNext(blobSet, &it);
            continue;
        }

        current = (Blob*)orderedSetIterNext(blobSet, &it);
    }
}

// Function to find a blob of at least the requested size, or split a larger one
Blob* allocateBlob(OrderedSet* blobSet, size_t requestedSize) {
    Blob query = {requestedSize, 0};
    Blob* found = (Blob*)orderedSetFindSmallestAtleast(blobSet, (byteArray)&query);

    if (found) {
        orderedSetRemove(blobSet, (byteArray)found);
        return found;
    }

// Step 1: Search for an available blob
Blob queryBlob = {requestedSize, 0};
Blob* foundBlob = (Blob*)orderedSetFindSmallestAtleast(blobSet, (byteArray)&queryBlob);

if (foundBlob) {
    // Exact match found, return it
    return foundBlob;
}

// Step 2: Search for a larger blob that can be split
Blob largerQueryBlob = {requestedSize + 1, 0};
Blob* largerBlob = (Blob*)orderedSetFindSmallestAtleast(blobSet, (byteArray)&largerQueryBlob);

if (largerBlob) {
    // Remove the larger blob from the set
    orderedSetRemove(blobSet, (byteArray)largerBlob);

    // Create the exact requested blob
    Blob allocatedBlob = {requestedSize, largerBlob->offset};

    // Create the remaining fragment
    Blob remainingBlob = {largerBlob->size - requestedSize, largerBlob->offset + requestedSize};

    // Insert the remaining part back into the tree
    orderedSetInsert(blobSet, (byteArray)&remainingBlob);

    // Return the allocated blob
    return &allocatedBlob;
}

// If no suitable blob is found, return NULL
return NULL;
}

int main() {
    createObjectPool(5, sizeof(AVLNode), NULL, &nodesSet);
    OrderedSet blobSet;
    createOrderedSet(&blobSet, sizeof(Blob), compareBlobs, Malloc, Free);
    
    Blob blobs[] = {
        {32, 100}, {64, 164}, {16, 50}, {32, 132}, {128, 300}
    };
    
    printf("Inserting blobs...\n");
    for (int i = 0; i < 5; i++) {
        orderedSetInsert(&blobSet, (char*)&blobs[i]);
    }

    Blob a = {33, 100};

    printf("\nExists : %d", orderedSetContains(&blobSet, (byteArray)&a));
    printBlob((byteArray)orderedSetFindSmallestAtleast(&blobSet, (byteArray)&a));

    printf("\nTree contents after insertion:\n");
    orderedSetTraverse(&blobSet, printBlob, TRAVERSAL_IN_ORDER);
    printf("\n\n");

    printf("Searching and merging adjacent blobs...\n");
    mergeAdjacentBlobs(&blobSet);

    printf("\nTree contents after merging:\n");
    orderedSetTraverse(&blobSet, printBlob, TRAVERSAL_IN_ORDER);
    printf("\n");

    printf("\nRequesting a blob of size 40...\n");
    Blob* allocated = allocateBlob(&blobSet, 40);
    if (allocated) {
        printf("Allocated blob: [Size: %zu, Offset: %zu]\n", allocated->size, allocated->offset);
    } else {
        printf("No suitable blob found.\n");
    }

    printf("\nTree contents after allocation:\n");
    orderedSetTraverse(&blobSet, printBlob, TRAVERSAL_IN_ORDER);
    printf("\n");

    destroyOrderedSet(&blobSet);
    return 0;
}
*/
