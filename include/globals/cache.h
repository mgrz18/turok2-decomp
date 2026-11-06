#ifndef GLOBALS_CACHE_H
#define GLOBALS_CACHE_H

/* The cache's sizes (Turok 3's hash.c / cache.c). */

#include "common.h"

extern s32 gHashSize;           /* RoundToPowerOfTwo(entries) (CHashTable__Construct) */
extern u32 gHashTableSize;      /* gHashSize * 2 */
extern u32 gCacheEntryCount;    /* 0x400, doubled with the expansion pak */

#endif
