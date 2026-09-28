#include "common.h"
#include "m2c_macros.h"

s32 func_00220260(s32, s32, s32);

M2C_UNK func_0026E110(void *, M2C_UNK, void *);     /* extern */
extern M2C_UNK D_800F7078;

void func_0025E888(void *arg0, void *arg1) {
    s32 temp_a0;
    void *sub;

    M2C_FIELD(arg1, s32 *, 4) = 0;
    M2C_FIELD(arg1, s8 *, 0x35) = -1;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x20000);
    temp_a0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0);
    if (((temp_a0 == 1) || (temp_a0 == 4)) && ((u32) (M2C_FIELD(arg1, u8 *, 0x34) - 0x35) < 7U)) {
        M2C_FIELD(arg1, s32 *, 0) = (s32) (M2C_FIELD(arg1, s32 *, 0) | 0x20);
    }
    sub = M2C_FIELD(arg0, void **, 0x14);
    if (!(M2C_FIELD(arg0, s32 *, 0xD4) & 0x80000)) {
        if (!(M2C_FIELD(sub, s32 *, 0x14) & 0x20)) {
            func_00220260((s32) &D_800F7078, (s32) arg0, 1);
        }
        func_0026E110(arg0, 1, arg0);
    }
}
