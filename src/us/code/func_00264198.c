#include "common.h"
#include "m2c_macros.h"

void func_0026426C(s32, s32);
s32 func_00264378(s32, s32);

extern M2C_UNK D_800B20B0;

void func_00264198(void *arg0, void *arg1) {
    void *temp_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x14);
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_800B20B0;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = func_00264378;
    M2C_FIELD(arg1, void (**)(s32, s32), 0x104) = func_0026426C;
    M2C_FIELD(arg1, s32 *, 0x11C) = 0;
    M2C_FIELD(arg1, s32 *, 0x120) = 0;
    temp_v0 = temp_v1 + 0x14;
    M2C_FIELD(arg1, f32 *, 0x64) = (f32) M2C_FIELD(temp_v0, f32 *, 0x40);
    M2C_FIELD(arg1, s32 *, 0x10C) = 0;
    M2C_FIELD(arg1, s32 *, 0x110) = 0;
    M2C_FIELD(arg1, s32 *, 0x114) = 0;
    M2C_FIELD(arg1, s32 *, 0x118) = 0;
    M2C_FIELD(arg1, f32 *, 0x124) = (f32) (M2C_FIELD(temp_v0, f32 *, 0x40) * 6.283186f);
    if (M2C_FIELD(temp_v1, s32 *, 0x14) & 4) {
        M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x2000);
    }
}
