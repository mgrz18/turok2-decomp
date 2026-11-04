#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ f32 key;
} SortItem;

/* qsort-style comparator on the f32 key at 0x4. */
s32 func_00226D30(SortItem *arg0, SortItem *arg1) {
    s32 r = 1;

    if (arg0->key > arg1->key) {
        r = -1;
    }
    return r;
}
