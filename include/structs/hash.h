#ifndef STRUCTS_HASH_H
#define STRUCTS_HASH_H

/* The cache's hash table (CHashTable__Construct to CHashTable__GetLongestChain;
 * Turok 3's hash.h). gHashTable holds gHashTableSize entries: the first
 * gHashSize * 2 are the buckets (indexed by a hash & gHashMask), the chains
 * link through next. value is the CCacheEntry filed under key (a ROM address).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

struct CCacheEntry;

typedef struct HashEntry {
    /* 0x00 */ u32 key;
    /* 0x04 */ struct CCacheEntry *value;
    /* 0x08 */ u32 index;
    /* 0x0C */ struct HashEntry *next;
} HashEntry; /* size 0x10 */

#endif
