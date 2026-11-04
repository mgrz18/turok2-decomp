#include "common.h"

/* A cached resource: its data, its key, a reference count and flags (0x100: referenced). */
typedef struct {
    /* 0x00 */ s32 data;
    /* 0x04 */ s32 key;
    /* 0x08 */ s32 refCount;
    /* 0x0C */ s32 flags;
} CacheEntry;

CacheEntry *func_002031E8(s32, s32);
s32 func_002062F8(s32, CacheEntry *);
s32 func_002063B4(s32, s32, s32, s32);
s32 func_00206498(s32, CacheEntry *);

CacheEntry *func_002060E0(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v0_2;
    CacheEntry *var_s0;

    var_s0 = func_002031E8(0, arg2);
    if (var_s0 != 0) {
        var_s0->refCount += 1;
        var_s0->flags |= 0x100;
        temp_v0 = func_002063B4(0, arg1, ((u32) arg2 >> 5) & 1, arg2);
        var_s0->data = temp_v0;
        if (temp_v0 != 0) {
            var_s0->key = arg1;
            var_s0->flags |= arg2;
            func_00206498(0, var_s0);
            return var_s0;
        }
        temp_v0_2 = var_s0->refCount - 1;
        var_s0->refCount = temp_v0_2;
        if (temp_v0_2 == 0) {
            var_s0->flags &= ~0x100;
        }
        func_002062F8(0, var_s0);
        var_s0 = NULL;
        goto block_6;
    }
block_6:
    return var_s0;
}
