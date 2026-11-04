#include "common.h"

/* The world (D_800F7078): only its two bit-section tables are used here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 bitSections;
    /* 0x70 */ s32 secondaryBitSections;
} World;

s32 func_002017D4(s32, s32);                    /* extern */
s32 func_00201818(s32, s32);                /* extern */

void func_002267A0(World *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_a0;

    if (arg1 != 0) {
        var_a0 = arg0->bitSections;
    } else {
        var_a0 = arg0->secondaryBitSections;
    }
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(var_a0, 0), arg3), arg2);
    func_002017D4(temp_v0, 0);
    func_00201818(temp_v0, 1);
    func_002017D4(temp_v0, 1);
}
