#include "common.h"
#include "structs/emitter.h"

/* qsort-style comparator: ascending by the float at 0x210. */
s32 Emitter_CompareHeightAsc(CEmitter **a, CEmitter **b) {
    s32 r = 1;
    if ((*a)->m_Height < (*b)->m_Height) {
        r = -1;
    }
    return r;
}
