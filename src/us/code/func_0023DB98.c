#include "common.h"

/* qsort comparator over records with a float key at 4. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ f32 key;
} KeyedRecord;

s32 func_0023DB98(KeyedRecord *arg0, KeyedRecord *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}
