#include "../include/orderedSet.h"
#include "../include/logger.h"
#include "../include/asserts.h"
#include "stdlib.h"


// - - - | Helper Functions On Nodes | - - - 


// - - - get height and balance - - - 

static i64 getHeight(AVLNode* NODE)  { return NODE ? NODE->height : 0; }
static i64 getBalance(AVLNode* NODE) { return NODE ? getHeight(NODE->left) - getHeight(NODE->right) : 0; }


// - - - create Node - - - 
static AVLNode* createNode(OrderedSet* SET, byteArray KEY)
{
  AVLNode* node = (AVLNode*) SET->allocator(sizeof(AVLNode));
  FORGE_ASSERT_MESSAGE(node, "[ORDERED SET] : Memory Allocation failed for AVL Node");

  node->key     = KEY;  
  node->left    = NULL;
  node->right   = NULL;
  node->height  = 1;

  memcpy(node->key, KEY, SET->keySize);
  return node;
}

// - - - rotation - - -
static AVLNode* rotateRight(AVLNode* Y)
{
  AVLNode* X    = Y->left;
  AVLNode* T2   = X->right;

  X->right      = Y;
  Y->left       = T2;

  Y->height     = 1 + getHeight(Y->left) > getHeight(Y->right) ? 
                        getHeight(Y->left) : 
                        getHeight(Y->right);
  X->height     = 1 + getHeight(X->left) > getHeight(X->right) ? 
                        getHeight(X->left) : 
                        getHeight(X->right);
  return X;
}

static AVLNode* rotateLeft(AVLNode* X)
{
  AVLNode* Y    = X->right;
  AVLNode* T2   = Y->left;

  Y->right      = X;
  X->right      = T2;

  X->height     = 1 + (getHeight(X->left) > getHeight(X->right) ? 
                        getHeight(X->left) : 
                        getHeight(X->right));
  Y->height     = 1 + (getHeight(Y->left) > getHeight(Y->right) ? 
                        getHeight(Y->left) : 
                        getHeight(Y->right));
  return Y;
}

// - - - find - - - 
static AVLNode* findMinNode(AVLNode* NODE)
{
  while (NODE->left) NODE = NODE->left;
  return NODE;
}

static AVLNode* findMaxNode(AVLNode* NODE)
{
  while (NODE->right) NODE = NODE->right;
  return NODE;
}

static bool containsNode(AVLNode* NODE, memoryCompare* COMPARE, void* KEY, u64 KEY_SIZE)
{
  if (NODE == NULL) return false;

  i64 cmp = COMPARE(KEY, NODE->key, KEY_SIZE);
  if (cmp < 0) return containsNode(NODE->left,  COMPARE, KEY, KEY_SIZE);
  if (cmp > 0) return containsNode(NODE->right, COMPARE, KEY, KEY_SIZE);
  return true;
}


// - - - insert and delete - - - 

static AVLNode* insertNode(OrderedSet* SET, AVLNode* NODE, byteArray KEY)
{
  if (!NODE) return createNode(SET, KEY);

  // - - - compare
  i8 cmp = SET->compare(KEY, NODE->key, SET->keySize);
  if        (cmp == 0)  return NODE;
  else if   (cmp < 0)   NODE->left  = insertNode(SET, NODE->left, KEY);
  else if   (cmp > 0)   NODE->right = insertNode(SET, NODE->right, KEY);

  // - - - update height 
  NODE->height = 1 + (getHeight(NODE->left) > getHeight(NODE->right) ? 
                      getHeight(NODE->left) : 
                      getHeight(NODE->right));

  // - - - balance the nodes
  int balance = getBalance(NODE);
  if (balance > 1  && SET->compare(KEY, NODE->left->key, SET->keySize)  < 0) return rotateRight(NODE);
  if (balance < -1 && SET->compare(KEY, NODE->right->key, SET->keySize) > 0) return rotateLeft(NODE);
  if (balance > 1  && SET->compare(KEY, NODE->left->key, SET->keySize) > 0)
  {
    NODE->left = rotateLeft(NODE->left);
    return rotateRight(NODE);
  }
  if (balance < -1 && SET->compare(KEY, NODE->right->key, SET->keySize) < 0)
  {
    NODE->right = rotateRight(NODE->right);
    return rotateLeft(NODE);
  }

  return NODE;
}

static AVLNode* deleteNode(OrderedSet* SET, AVLNode* NODE, byteArray KEY)
{
  if (NODE == NULL) return NULL;

  // - - - compare 
  int cmp                       = SET->compare(KEY, NODE->key, SET->keySize);
  if (cmp < 0)      NODE->left  = deleteNode(SET, NODE->left,  KEY);
  else if (cmp > 0) NODE->right = deleteNode(SET, NODE->right, KEY);
  else 
  {
    if (NODE->left == NULL || NODE->right == NULL)
    {
      AVLNode* temp = NODE->left ? NODE->left : NODE->right;
      if (temp == NULL)
      {
        temp = NODE;
        NODE = NULL;
      }
      else 
      {
        *NODE = *temp;
      }
      SET->deallocator(temp);
    }
    else 
    {
      AVLNode* temp = findMinNode(NODE->right);
      NODE->key = temp->key;
      NODE->right = deleteNode(SET, NODE->right, temp->key);
    }
  }

  if (NODE == NULL) return NULL;
  NODE->height = 1 + (getHeight(NODE->left) > getHeight(NODE->right) ? 
                      getHeight(NODE->left) :
                      getHeight(NODE->right));

  // - - - balance 
  i64 balance = getBalance(NODE);
  if (balance > 1  && getBalance(NODE->left)  >= 0) return rotateRight(NODE);
  if (balance < -1 && getBalance(NODE->right) <= 0) return rotateLeft(NODE);
  if (balance > 1  && getBalance(NODE->left)   < 0)   
  {
    NODE->left = rotateLeft(NODE->left);
    return rotateRight(NODE);
  }
  if (balance < -1 && getBalance(NODE->right) > 0)  
  {
    NODE->right = rotateRight(NODE->right);
    return rotateLeft(NODE);
  }

  return NODE;
}

static void deleteSetNodes(OrderedSet* SET, AVLNode* NODE)
{
  if (NODE)
  {
    deleteSetNodes(SET, NODE->left);
    deleteSetNodes(SET, NODE->right);
    SET->deallocator(NODE);
  }
}


// - - - | Ordered Set Functions | - - - 


// - - - create and destroy - - - 

void createOrderedSet(OrderedSet* SET, u64 KEY_SIZE, memoryCompare* COMPARE, memoryAllocate* MALLOC, memoryDeallocate* FREE)
{
  FORGE_ASSERT_MESSAGE(SET,      "[ORDERED SET] : Cannot initialize a null OrderedSet");
  FORGE_ASSERT_MESSAGE(KEY_SIZE, "[ORDERED SET] : Cannot have a key size of less than 1 byte");

  SET->compare      = COMPARE;
  SET->allocator    = MALLOC;
  SET->deallocator  = FREE;
  SET->keySize      = KEY_SIZE;
  SET->root         = NULL;


  if (COMPARE == NULL)
  {
    FORGE_LOG_WARNING("[ORDERED SET] : No memory comparison function passed. Will use 'memcmp' from stdlib");
    SET->compare = memcmp;
  }
  if (MALLOC == NULL)
  {
    FORGE_LOG_WARNING("[ORDERED SET] : No memory allocation function passed. Will use 'malloc' from stdlib");
    SET->allocator = malloc;
  }
  if (FREE == NULL)
  {
    FORGE_LOG_WARNING("[ORDERED SET] : No memory deallocation function passed. Will use 'free' from stdlib");
    SET->deallocator = free;
  }
}

void destroyOrderedSet(OrderedSet* SET)
{
  FORGE_ASSERT_MESSAGE(SET, "[ORDERED SET] : Cannot destroy a NULL ordered set");
  deleteSetNodes(SET, SET->root);
  SET->root = NULL;
}


// - - - insert and delete - - - 

void orderedSetInsert(OrderedSet* SET, byteArray KEY)
{
  FORGE_ASSERT_MESSAGE(SET, "[ORDERED SET] : Cannot insert in a NULL ordered set");
  FORGE_ASSERT_MESSAGE(KEY, "[ORDERED SET] : Cannot insert a NULL key");

  SET->root = insertNode(SET, SET->root, KEY);
}

void orderedSetRemove(OrderedSet* SET, byteArray KEY)
{
  FORGE_ASSERT_MESSAGE(SET, "[ORDERED SET] : Cannot remove from a NULL ordered set");
  FORGE_ASSERT_MESSAGE(KEY, "[ORDERED SET] : Cannot remove a NULL key");

  SET->root = deleteNode(SET, SET->root, KEY);
}

bool orderedSetContains(OrderedSet* SET, byteArray KEY)
{
  FORGE_ASSERT_MESSAGE(SET, "[ORDERED SET] : Cannot remove from a NULL ordered set");
  FORGE_ASSERT_MESSAGE(KEY, "[ORDERED SET] : Cannot remove a NULL key");

  return containsNode(SET->root, SET->compare, KEY, SET->keySize);
}
