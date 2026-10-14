#include "common.h"
#include "m2c_macros.h"

s32 func_0020D908(s32, s32);
s32 func_0020F7EC(s32);
s32 func_002333C0(s32, s32, s32);

M2C_UNK func_0021034C(M2C_UNK *, f32, f32, f32);    /* extern */
extern f32 D_800A653C;
extern f32 D_800A6540;

void func_00236C64(s32 arg0, void *arg1, s32 arg2, s32 arg3) {
    u8 sp10[0x40];
    f32 var_f0;
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg1, void **, 0x1C);
    if ((temp_s0 == arg2) && (M2C_FIELD(arg1, f32 *, 0x24) > 0.0f)) {
        func_0020D908((s32) sp10, arg3);
        var_f0 = D_800A653C;
        if (M2C_FIELD(M2C_FIELD(temp_s0, void **, 0xF8), s32 *, 0x14) != 0) {
            var_f0 = D_800A6540;
        }
        func_0021034C(sp10, var_f0, var_f0, var_f0);
        func_0020F7EC((s32) sp10);
        func_002333C0(arg0, (s32) arg1, (s32) sp10);
    }
}
