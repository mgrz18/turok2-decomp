/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/802A35C0_6.CPP, func_802A56C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/swoosh.h"

/* count vertices from this frame's ribbon buffer (CSwooshSystem.m_RibbonVtx), or 0 when fewer are left. */
sptr RibbonVtxPool_Alloc(void *arg0, s32 arg1) {
    CSwooshSystem *pool = arg0;
    s32 avail;
    sptr head;
    avail = pool->m_RibbonVtxLeft;
    if (avail < arg1) {
        return 0;
    }
    head = (sptr)pool->m_pRibbonVtx;
    pool->m_RibbonVtxLeft = avail - arg1;
    pool->m_pRibbonVtx += arg1;
    return head;
}
