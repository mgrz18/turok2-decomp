#include "common.h"
#include "m2c_macros.h"

s32 func_00257D48(s32, s32);
s32 func_00258754(s32, s32);
s32 func_0025883C(s32, s32, s32);

extern M2C_UNK D_800AE394;

void func_00258964(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, M2C_UNK **, 0x2C) = &D_800AE394;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF0) = func_00257D48;
    M2C_FIELD(arg1, s32 (**)(s32, s32, s32), 0xF4) = func_0025883C;
    M2C_FIELD(arg1, s32 *, 0x10C) = 0;
    M2C_FIELD(arg1, s32 *, 0x110) = 0;
    M2C_FIELD(arg1, s32 *, 0x118) = 0;
    M2C_FIELD(arg1, s32 *, 0x120) = 0;
    M2C_FIELD(arg1, s32 *, 0x124) = 0;
    M2C_FIELD(arg1, s32 *, 0x128) = 0;
    M2C_FIELD(arg1, s32 *, 0x12C) = 0;
    M2C_FIELD(arg1, s32 *, 0x130) = 0;
    M2C_FIELD(arg1, f32 *, 0x114) = 1.0f;
    func_00258754(arg0, (s32) arg1);
}
