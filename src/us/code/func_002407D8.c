#include "common.h"

#ifdef TARGET_PORT
/* the records sorted are {pointer, f32} (ModelHit, InstanceHit): the key after a host pointer */
typedef struct {
    void *unk0;
    f32 key;
} SortItem;
#else
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ f32 key;
} SortItem;
#endif

/* qsort-style comparator on the f32 key at 0x4. */
s32 Collision_CompareInstanceHits(SortItem *arg0, SortItem *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}
