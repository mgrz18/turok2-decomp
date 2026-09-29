#include "common.h"
#include "m2c_macros.h"

s32 func_00248720(s32, s32);
s32 func_00253DE0(s32, s32);
s32 func_00253EB8(s32, s32);

extern M2C_UNK D_800A72C8;
extern M2C_UNK D_800AF618;

void func_00259040(void *arg0, void *arg1) {
    s32 temp_s1;

    temp_s1 = M2C_FIELD(arg0, s32 *, 0x1A8);
    M2C_FIELD(arg1, s32 *, 0x64) = 0;
    func_00248720(M2C_FIELD(arg0, s32 *, 0x1A8), 0x12B);
    func_00253DE0(temp_s1, 0x1A5);
    func_00253EB8(temp_s1, 0x1A7);
    M2C_FIELD(arg1, f32 *, 0x118) = (f32) (M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(M2C_FIELD(arg0, s32 *, 0x1A8), s16 *, 0x996) * 4)))), f32 *, 0x18) * M2C_FIELD(&D_800A72C8, f32 *, 4));
}
