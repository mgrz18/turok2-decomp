#include "common.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 key;
} SortEntry;

/* Sort callback: -1 when arg0's key is the lower, else 1. */
s32 SortRecord_CompareDescending(SortEntry *arg0, SortEntry *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}
