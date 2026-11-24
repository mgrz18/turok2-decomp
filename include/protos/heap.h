/* CHeap, the engine allocator (src/us/code/CHeap), and the CDecompressor request call:
 * the signatures of the definitions, for the modules that call them (round 28, part q2). */

#ifndef PROTOS_HEAP_H
#define PROTOS_HEAP_H

#include "common.h"
#include "ultra_types.h"
#include "structs/heap.h"

struct CDecompressor;
struct CCacheEntry;

void *CHeap__Alloc(CHeap *heap, s32 request);
s32 CHeap__Free(CHeap *heap, u8 *data);
s32 CDecompressor__DecompressAsync(struct CDecompressor *thisx, struct CCacheEntry *entry, OSMesgQueue *replyQueue);

#endif
