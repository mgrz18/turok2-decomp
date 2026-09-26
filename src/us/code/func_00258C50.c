#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);
s32 func_0024E700(s32, s32);
s32 func_00257EA0(s32, s32);

extern f32 D_800A7298;
extern M2C_UNK D_800AF618;

void func_00258C50(void *arg0, void *arg1) {
    s16 temp_a1;
    void *temp_s1;

    temp_s1 = M2C_FIELD(arg0, void **, 0x1A8);
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        if ((M2C_FIELD(arg1, f32 *, 0x118) <= 0.0f) && (temp_a1 = M2C_FIELD(temp_s1, s16 *, 0x996), ((M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + ((temp_a1 << 2))))), s32 *, 0x14) & 4) != 0)) && (func_0024E700((s32) temp_s1, (s32) temp_a1) != 0) && (func_00257EA0((s32) arg0, (s32) arg1) != 0)) {
            if ((M2C_FIELD(temp_s1, s32 *, 0xA64) & 0x2000) && (M2C_FIELD(M2C_FIELD(temp_s1, void **, 0x518), u8 *, 0x3E) == 0)) {
                M2C_FIELD(arg1, s8 *, 0xC7) = 0;
                M2C_FIELD(arg1, s8 *, 0x35) = -1;
                M2C_FIELD(arg1, s8 *, 0x34) = -1;
                func_00243414((s32) arg0, (s32) arg1, 4);
                M2C_FIELD(arg1, f32 *, 0x118) = (f32) (M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(M2C_FIELD(arg0, void **, 0x1A8), s16 *, 0x996) << 2)))), f32 *, 0x18) * D_800A7298);
            } else {
                goto block_9;
            }
        } else {
block_9:
            func_00243414((s32) arg0, (s32) arg1, 2);
        }
        M2C_FIELD(temp_s1, s32 *, 0xB2C) = 1;
        M2C_FIELD(temp_s1, s32 *, 0xB30) = 0;
    }
}
