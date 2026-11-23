#include "common.h"
#include "structs/emitter.h"

/* qsort-style comparator on the float at 0x210 of the pointed-to objects. */
s32 Emitter_CompareHeightDesc(CEmitter **arg0, CEmitter **arg1) {
    s32 r = 1;
    if ((*arg0)->m_Height > (*arg1)->m_Height) {
        r = -1;
    }
    return r;
}
