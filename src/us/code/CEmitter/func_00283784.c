#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x210];
    /* 0x210 */ f32 key;
} Sortable;

/* qsort-style comparator on the float at 0x210 of the pointed-to objects. */
s32 Emitter_CompareHeightDesc(Sortable **arg0, Sortable **arg1) {
    s32 r = 1;
    if ((*arg0)->key > (*arg1)->key) {
        r = -1;
    }
    return r;
}
