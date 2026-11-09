/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/TEXTURE/80254CE4.CPP, func_80254ED0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
#include "structs/cache.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"

static __inline__ CCacheEntry *take(s32 active) {
 CCacheEntry *node=gCache_cacheFreeList.m_pHead;
 if(node) {CList__Remove(&gCache_cacheFreeList,node); node->flags=active;CList__AddHead(&gCache_cacheUsedList,node);}
 return node;
}
void *CCache__AllocCacheEntry(s32 unused,s32 flags) {
 CCacheEntry *node=take(1);
 if (!node) {
  s32 active=1;
  flags &= 0x10;
  while(CCache__FreeOldestMemEntry(0,2,flags==0)) {node=take(active);if(node)break;}
 }
 return node;
}
