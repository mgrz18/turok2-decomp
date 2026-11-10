/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254E30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cache.h"
#include "protos/cache.h"

void *CCache__TakeFreeCacheEntry(void) {
    extern void CList__Remove(void *, sptr);
    extern s32 CList__AddHead(void *, sptr);
    void **p = &gCache_cacheFreeList;
    void *temp_s0 = *p;
    if (temp_s0 != 0) {
        CList__Remove(p, (sptr) temp_s0);
        ((ResourceLoadRequest *)temp_s0)->flags = 1;
        CList__AddHead(((char *)p + sizeof(CList)), (sptr) temp_s0);
    }
    return temp_s0;
}
