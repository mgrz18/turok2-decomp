#include "common.h"
#include "m2c_macros.h"
#include "rw/abu_protos.h"

f32 func_002119FC(f32 arg0, f32 arg1);
s32 func_002613D8(s32, s32);

extern f32 D_800A7A34;
extern f32 D_800A7A38;

void func_00260FF8(s32 arg0, void *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = D_800A7A34;
    temp_f21 = D_800A7A38;
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xFC) = func_002613D8;
    M2C_FIELD(arg1, f32 *, 0x6C) = func_002119FC(temp_f20, temp_f21);
    M2C_FIELD(arg1, f32 *, 0x70) = func_002119FC(temp_f20, temp_f21);
    M2C_FIELD(arg1, f32 *, 0x74) = func_002119FC(temp_f20, temp_f21);
}
