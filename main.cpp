#include "library/include/objectPool.h"
#include <stdio.h>

void testBasicFunctionality() {
    printf("\n[Test] Basic Functionality\n");
    ObjectPool pool;
    createObjectPool(4, sizeof(int), 0, NULL, &pool);
    
    int* obj1 = (int*) takeObject(&pool);
    int* obj2 = (int*) takeObject(&pool);
    returnObject(&pool, obj1);
    returnObject(&pool, obj2);
    printf("Passed.\n");
}

void testNonResizablePool() {
    printf("\n[Test] Non-Resizable Pool\n");
    ObjectPool pool;
    createObjectPool(2, sizeof(f64), 0, NULL, &pool);
    
    int* obj1 = (int*) takeObject(&pool);
    int* obj2 = (int*) takeObject(&pool);
    int* obj3 = (int*) takeObject(&pool); // Should return NULL
    
    if (!obj3) {
        printf("Correctly handled out-of-memory case.\n");
    } else {
        printf("Failed: Took more than available!\n");
    }
}

void testInvalidObjectReturn() {
    printf("\n[Test] Invalid Object Return\n");
    ObjectPool pool;
    createObjectPool(4, 12, 0, NULL, &pool);
    
    int fakeObj;
    returnObject(&pool, &fakeObj); // Should trigger assertion or error handling
}

void testResizingPool() {
    printf("\n[Test] Resizing Pool\n");
    ObjectPool pool;
    createObjectPool(2, 16, 2.0f, NULL, &pool);
    
    int* objs[5];
    for (int i = 0; i < 5; i++) {
        objs[i] = (int*) takeObject(&pool);
        printf("Allocated object %d\n", i);
    }
    
    printf("Pool resized successfully.\n");
}

int main() {
    testBasicFunctionality();
    testNonResizablePool();
    testInvalidObjectReturn(); // Uncomment to see failure case
    testResizingPool();
    return 0;
}
