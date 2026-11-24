/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254E30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/cache.h"
#include "protos/list.h"
#include "protos/cache.h"

/* Takes the head of the free request list (flags 1) onto the used list, the list after it. */
void *CCache__TakeFreeCacheEntry(void) {
    CList *p = &gCache_cacheFreeList;
    CCacheEntry *e = p->m_pHead;

    if (e != 0) {
        CList__Remove(p, e);
        e->flags = 1;
        CList__AddHead(p + 1, e);
    }
    return e;
}
