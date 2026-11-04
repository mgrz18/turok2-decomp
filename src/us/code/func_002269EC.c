#include "common.h"
#include "rw/abu_protos.h"

/* Only the field read here: a resource walked with func_002017D4. */
typedef struct {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ s32 *unk70;
} BitTableOwner;

char *func_002017D4(s32 *, s32);
s32 func_00201818(s32, s32);

s32 func_002269EC(BitTableOwner *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(arg0->unk70, 0), arg1), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    temp_a0 = func_002017D4(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}
