#ifndef iiit_CHUNK_HPP
#define iiit_CHUNK_HPP

#include "common.hpp"

// - - - I would avoid 1.5 since I am NOT WORKING WITH FLOATS
#define _GROW_CAPACITY(mCAPACITY) \
 ((mCAPACITY) < 8 ? 8 : (mCAPACITY) * 2)

#define _GROW_VEC(mTYPE, mPOINTER, mOLDCOUNT, mNEWCOUNT)
// - - - Defining the byte structure here.
typedef enum{
 OP_RETURN,
} OpCode;

// - - - Defining the chunk structure here. Need to make it dynamic!
struct _CHUNK{
 size_t mCOUNT;   // - - -Using size_t instead of u32 or something because I don't know platforms my guy
 size_t mCAPACITY;
 u8* mCODE;

 _CHUNK();
 void _writeCHUNK(u8 _BYTE);
};

using CHUNK = struct _CHUNK;
#endif
