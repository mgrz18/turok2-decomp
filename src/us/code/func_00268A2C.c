#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 targetId;
} Link;

s32 func_0022425C(s32 *, s32);                  /* extern */
extern s32 D_800F7078;

s32 func_00268A2C(Link *arg0) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = arg0->targetId;
    var_v0 = 0;
    if (temp_a1 != 0) {
        var_v0 = func_0022425C(&D_800F7078, temp_a1);
    }
    return var_v0;
}
