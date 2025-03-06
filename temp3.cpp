#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "library/include/orderedSet.h"
#include "library/include/logger.h"
#include "library/include/asserts.h"

// Define the Blob structure
typedef struct {
    u64 size;
    u64 offset;
} Blob;

// Custom memory compare function for Blobs (comparing by offset)
int blobCompare(const void* a, const void* b, unsigned long size) {
    const Blob* blobA = (const Blob*)a;
    const Blob* blobB = (const Blob*)b;
    
    if (blobA->offset < blobB->offset) return -1;
    if (blobA->offset > blobB->offset) return 1;
    return 0;
}

// Callback for traversal to print Blob info
void printblob(byteArray data) {
    Blob* blob = (Blob*)data;
    printf("Blob: offset=%llu, size=%llu\n", 
           (unsigned long long)blob->offset, 
           (unsigned long long)blob->size);
}

// Function to verify the AVL tree property - each node's left subtree has all keys less than
// the node's key, and right subtree has all keys greater
bool verifyBSTProperty(AVLNode* node, memoryCompare compare, u64 keySize) {
    if (node == NULL) return true;
    
    // Check left subtree
    if (node->left) {
        // All nodes in left subtree must be less than current node
        AVLNode* maxNode = node->left;
        while (maxNode->right) maxNode = maxNode->right;
        
        if (compare(maxNode->key, node->key, keySize) >= 0) return false;
    }
    
    // Check right subtree
    if (node->right) {
        // All nodes in right subtree must be greater than current node
        AVLNode* minNode = node->right;
        while (minNode->left) minNode = minNode->left;
        
        if (compare(minNode->key, node->key, keySize) <= 0) return false;
    }
    
    // Recursively check children
    return verifyBSTProperty(node->left, compare, keySize) && 
           verifyBSTProperty(node->right, compare, keySize);
}

// Function to verify AVL balance property - heights of left and right subtrees
// differ by at most 1
bool verifyBalanceProperty(AVLNode* node) {
    if (node == NULL) return true;
    
    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right ? node->right->height : 0;
    
    // Check balance factor
    int balance = leftHeight - rightHeight;
    if (balance < -1 || balance > 1) return false;
    
    // Verify height is correct
    int maxChildHeight = leftHeight > rightHeight ? leftHeight : rightHeight;
    if (node->height != maxChildHeight + 1) return false;
    
    // Recursively check children
    return verifyBalanceProperty(node->left) && verifyBalanceProperty(node->right);
}

// Function to randomly generate blobs
Blob* generateRandomBlob() {
    Blob* blob = (Blob*)malloc(sizeof(Blob));
    blob->offset = (u64)rand() % 10000;  // Random offset between 0-9999
    blob->size = (u64)rand() % 1000 + 1; // Random size between 1-1000
    return blob;
}

int main() {
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    
    // Create an ordered set for Blobs
    OrderedSet blobSet;
    createOrderedSet(&blobSet, sizeof(Blob), blobCompare, malloc, free);
    
    printf("=== Testing OrderedSet with Blob structures ===\n\n");
    
    // Generate some random blobs for testing
    const int NUM_BLOBS = 20;
    Blob* blobs[NUM_BLOBS];
    
    printf("Generating %d random blobs...\n", NUM_BLOBS);
    for (int i = 0; i < NUM_BLOBS; i++) {
        blobs[i] = generateRandomBlob();
        printf("Generated Blob %d: offset=%llu, size=%llu\n", 
               i, (unsigned long long)blobs[i]->offset, (unsigned long long)blobs[i]->size);
    }
    
    // Test insertion - insert in random order
    printf("\n=== Inserting blobs in random order ===\n");
    for (int i = 0; i < NUM_BLOBS; i++) {
        printf("Inserting Blob %d: offset=%llu, size=%llu\n", 
               i, (unsigned long long)blobs[i]->offset, (unsigned long long)blobs[i]->size);
        
        orderedSetInsert(&blobSet, (byteArray)blobs[i]);
        
        // Verify tree properties after each insertion
        if (!verifyBSTProperty(blobSet.root, blobCompare, sizeof(Blob))) {
            printf("ERROR: BST property violated after inserting Blob %d\n", i);
            return 1;
        }
        
        if (!verifyBalanceProperty(blobSet.root)) {
            printf("ERROR: AVL balance property violated after inserting Blob %d\n", i);
            return 1;
        }
    }
    
    // Print tree in-order (should be sorted by offset)
    printf("\n=== Tree contents (in-order traversal) ===\n");
    orderedSetTraverse(&blobSet, printblob, TRAVERSAL_IN_ORDER);
    
    // Test removal - remove in random order
    printf("\n=== Removing blobs in random order ===\n");
    
    // Create a shuffled array of indexes for random removal
    int indexes[NUM_BLOBS];
    for (int i = 0; i < NUM_BLOBS; i++) {
        indexes[i] = i;
    }
    
    // Simple shuffle
    for (int i = NUM_BLOBS - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indexes[i];
        indexes[i] = indexes[j];
        indexes[j] = temp;
    }
    
    // Remove blobs in shuffled order
    for (int i = 0; i < NUM_BLOBS; i++) {
        int idx = indexes[i];
        printf("Removing Blob %d: offset=%llu, size=%llu\n", 
               idx, (unsigned long long)blobs[idx]->offset, (unsigned long long)blobs[idx]->size);
        
        // Use our new function that returns the removed key
        byteArray removedBlob = orderedSetRemove(&blobSet, (byteArray)blobs[idx]);
        
        if (removedBlob) {
            Blob* removed = (Blob*)removedBlob;
            printf("Successfully removed: offset=%llu, size=%llu\n", 
                   (unsigned long long)removed->offset, (unsigned long long)removed->size);
            
            // Verify it matches what we expected to remove
            if (removed->offset != blobs[idx]->offset || removed->size != blobs[idx]->size) {
                printf("ERROR: Removed blob doesn't match expected blob!\n");
                return 1;
            }
        } else {
            printf("ERROR: Failed to remove blob!\n");
            return 1;
        }
        
        // Verify tree properties after each removal
        if (blobSet.root != NULL) {
            if (!verifyBSTProperty(blobSet.root, blobCompare, sizeof(Blob))) {
                printf("ERROR: BST property violated after removing Blob %d\n", idx);
                return 1;
            }
            
            if (!verifyBalanceProperty(blobSet.root)) {
                printf("ERROR: AVL balance property violated after removing Blob %d\n", idx);
                return 1;
            }
        }
        
        // Print remaining tree after each removal
        printf("Remaining tree:\n");
        orderedSetTraverse(&blobSet, printblob, TRAVERSAL_IN_ORDER);
        printf("\n");
    }
    
    // Verify the tree is empty
    if (getOrderedSetSize(&blobSet) != 0) {
        printf("ERROR: Tree should be empty but has size %llu\n", 
               (unsigned long long)getOrderedSetSize(&blobSet));
        return 1;
    }
    
    // Clean up
    for (int i = 0; i < NUM_BLOBS; i++) {
        free(blobs[i]);
    }
    destroyOrderedSet(&blobSet);
    
    printf("\n=== Test completed successfully! ===\n");
    return 0;
}
