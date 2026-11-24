/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/80405454_4.CPP, func_80405CDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/frame.h"
#include "structs/cache.h"

extern void CCache__Deallocate(sptr heap, CMemEntry *handle);                     /* free */

/* Frees the graphics pool's cache blocks (as GfxPool_Alloc does first) and clears it. */
void GfxPool_Free(CGfxPool *pool) {
    s32 n;
    s32 j;

    if (pool->gfx != 0) {
        CCache__Deallocate(0, pool->gfx);
    }
    if (pool->dynamic != 0) {
        CCache__Deallocate(0, pool->dynamic);
    }
    if (pool->depth != 0) {
        CCache__Deallocate(0, pool->depth);
    }
    if (pool->vtx != 0) {
        CCache__Deallocate(0, pool->vtx);
    }
    n = 0;
    do {
        if (pool->fb[n] != 0) {
            CCache__Deallocate(0, pool->fb[n]);
        }
        n++;
    } while (n < 3);
    j = 2;
    pool->ready = 0;
    pool->gfx = 0;
    pool->dynamic = 0;
    pool->depth = 0;
    pool->vtx = 0;
    do {
        pool->fb[j] = 0;
        j--;
    } while (j >= 0);
}
