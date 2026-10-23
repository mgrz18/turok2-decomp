#include "common.h"
#include "m2c_macros.h"

s32 func_00266570(s32, s32, s32);

extern M2C_UNK D_800B2410;
extern M2C_UNK func_0026609C;

void func_002660D0(void *arg0, void *arg1) {
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_800B2410;
    M2C_FIELD(arg1, M2C_UNK **, 0xF0) = &func_0026609C;
    M2C_FIELD(arg1, s32 (**)(s32, s32, s32), 0xF4) = func_00266570;
    M2C_FIELD(arg1, s32 *, 0x10C) = 0;
    M2C_FIELD(arg1, s32 *, 0x110) = 0;
    M2C_FIELD(arg1, s32 *, 0x114) = 0;
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x14) & 1) {
        M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x10000);
        return;
    }
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & 0xFFFEFFFF);
}
