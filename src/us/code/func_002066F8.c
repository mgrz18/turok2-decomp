/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/TEXTURE/80254CE4.CPP, func_80254ED0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
#include "structs/cache.h"

extern s32 CCache__FreeOldestMemEntry(s32 unused0, s32 unused1, s32 keep_recent);
extern char D_800D8D7C;
void CList__AddHead(void *list, void *node);
extern CList D_800D8D68;
extern void CList__Remove(void *list, void *node);
static __inline__ CCacheEntry *take(s32 active) {
 CCacheEntry *node=D_800D8D68.m_pHead;
 if(node) {CList__Remove(&D_800D8D68,node); node->flags=active;CList__AddHead(&D_800D8D7C,node);}
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
