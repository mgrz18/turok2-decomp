#include "common.h"
#include "m2c_macros.h"

s32 func_002026EC(s32, s32);
s32 func_002062F8(s32, s32);
s32 func_002065F8(s32, s32);

extern M2C_UNK D_800D1CC0;

void func_002061B8(s32 arg0, void *arg1) {
    s32 temp_v0;

    M2C_FIELD(arg1, s32 *, 0xC) = (s32) (M2C_FIELD(arg1, s32 *, 0xC) & ~2);
    if (M2C_FIELD(arg1, s32 *, 8) != 0) {
loop_1:
        do {
            temp_v0 = M2C_FIELD(arg1, s32 *, 8) - 1;
            M2C_FIELD(arg1, s32 *, 8) = temp_v0;
            if (temp_v0 != 0) {
                goto loop_1;
            }
            M2C_FIELD(arg1, s32 *, 0xC) = (s32) (M2C_FIELD(arg1, s32 *, 0xC) & ~0x100);
        } while (M2C_FIELD(arg1, s32 *, 8) != 0);
    }
    if (!(M2C_FIELD(arg1, s32 *, 0xC) & 0x702)) {
        func_002065F8(0, (s32) arg1);
        func_002026EC((s32) &D_800D1CC0, M2C_FIELD(arg1, s32 *, 0));
        func_002062F8(0, (s32) arg1);
    }
}
