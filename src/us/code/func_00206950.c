#include "common.h"

/* A pool handing out 0x40-byte blocks. */
typedef struct {
    /* 0x0 */ u8 pad0[8];
    /* 0x8 */ s32 remaining;
    /* 0xC */ s32 next;
} BlockPool;

/* Takes arg1 blocks off the pool; 0 if there are not enough. */
s32 func_00206950(BlockPool *arg0, s32 arg1) {
    s32 var_a2;

    var_a2 = 0;
    if (arg0->remaining >= arg1) {
        var_a2 = arg0->next;
        arg0->next = (s32) (var_a2 + (arg1 << 6));
        arg0->remaining = (s32) (arg0->remaining - arg1);
    }
    return var_a2;
}
