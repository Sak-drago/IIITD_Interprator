#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library/include/treeSet.h"

// Helper function to print the tree in order
void printInOrder(AVLNode* node)
{
    if (!node) return;
    printInOrder(node->left);
    printf("%d ", *(int*)node->key);
    printInOrder(node->right);
}

// Function to dynamically allocate an integer
byteArray allocateInt(int value)
{
    int* ptr = (int*)malloc(sizeof(int));
    if (!ptr)
    {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }
    *ptr = value;
    return (byteArray)ptr;
}

int main()
{
    OrderedSet set;
    
    // Initialize set without custom functions (uses malloc, free, memcmp)
    createOrderedSet(&set, sizeof(int), NULL, NULL, NULL);

    // Insert dynamically allocated integers
    int values[] = {50, 30, 70, 20, 40, 60, 80, 90, 10, 35};
    int count = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < count; i++)
    {
        byteArray key = allocateInt(values[i]);
        orderedSetInsert(&set, key);
    }

    // Print in order
    printf("Tree contents (In-order): ");
    printInOrder(set.root);
    printf("\n");

    // Test contains function
    int searchValues[] = {40, 100}; // 40 exists, 100 does not
    for (int i = 0; i < 2; i++)
    {
        int key = searchValues[i];
        printf("Contains %d? %s\n", key, orderedSetContains(&set, (byteArray)&key) ? "Yes" : "No");
    }

    // Delete some elements
    int deleteValues[] = {30, 50, 90};
    for (int i = 0; i < 3; i++)
    {
        int key = deleteValues[i];
        printf("Deleting %d...\n", key);
        orderedSetRemove(&set, (byteArray)&key);
    }

    // Print tree after deletion
    printf("Tree contents after deletions: ");
    printInOrder(set.root);
    printf("\n");

    // Destroy set
    destroyOrderedSet(&set);

    // Memory leak check (use valgrind when running)
    printf("Test complete! Run with valgrind to check for memory leaks.\n");

    return 0;
}
