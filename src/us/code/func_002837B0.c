#include "common.h"

typedef struct {
    /* 0x000 */ char pad[0x210];
    /* 0x210 */ f32 key;
} Sortable;

/* qsort-style comparator: ascending by the float at 0x210. */
s32 func_002837B0(Sortable **a, Sortable **b) {
    s32 r = 1;
    if ((*a)->key < (*b)->key) {
        r = -1;
    }
    return r;
}
