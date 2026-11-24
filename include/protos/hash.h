/* CHashTable, the cache's request table (src/us/code/CHashTable): the signatures of the
 * definitions, for CCache (round 28, part q2). */

#ifndef PROTOS_HASH_H
#define PROTOS_HASH_H

#include "common.h"

struct CCacheEntry;

void CHashTable__Construct(s32 capacity);
void CHashTable__Insert(u32 key, struct CCacheEntry *value);
void CHashTable__Remove(u32 key);
void CHashTable__GetLongestChain(void);

#endif
