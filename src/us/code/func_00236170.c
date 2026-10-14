#include "common.h"
#include "m2c_macros.h"

s32 func_0020D908(s32, s32);
s32 func_0020F7EC(s32);
s32 func_002333C0(s32, s32, s32);

M2C_UNK func_0021034C(M2C_UNK *, f32, f32, f32);    /* extern */
extern f32 D_800A6514;
extern f32 D_800A6518;

void func_00236170(void *arg0, void *arg1, s32 arg2) {
    u8 sp10[0x40];
    f32 temp_f0;
    f32 var_f0;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x7520);
    if (var_s0 != NULL) {
        do {
            if (M2C_FIELD(var_s0, s32 *, 0x1C) == arg1) {
                temp_f0 = M2C_FIELD(var_s0, f32 *, 0x24);
                if (temp_f0 > 0.0f) {
                    if (temp_f0 > 0.0f) {
                        func_0020D908((s32) sp10, arg2);
                        var_f0 = D_800A6514;
                        if (M2C_FIELD(M2C_FIELD(arg1, void **, 0xF8), s32 *, 0x14) != 0) {
                            var_f0 = D_800A6518;
                        }
                        func_0021034C(sp10, var_f0, var_f0, var_f0);
                        func_0020F7EC((s32) sp10);
                        func_002333C0((s32) arg0, (s32) var_s0, (s32) sp10);
                    }
                }
            }
            var_s0 = M2C_FIELD(var_s0, void **, 4);
        } while (var_s0 != NULL);
    }
}
