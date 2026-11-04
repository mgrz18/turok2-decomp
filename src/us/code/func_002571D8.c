#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ f32 key;
} Sortable;

/* qsort-style comparator on the float at 0xC. */
s32 func_002571D8(Sortable *arg0, Sortable *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}
