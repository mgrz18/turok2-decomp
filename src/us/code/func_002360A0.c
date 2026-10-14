#include "common.h"
#include "m2c_macros.h"

s32 func_0020F7EC(s32);
s32 func_002333C0(s32, s32, s32);

M2C_UNK func_0020FF80(M2C_UNK *, s32);              /* extern */
M2C_UNK func_0021034C(M2C_UNK *, f32, f32, f32);    /* extern */

void func_002360A0(void *arg0, s32 arg1, s32 arg2) {
    u8 sp10[0x40];
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x7520);
    if (var_s0 != NULL) {
        do {
            if ((M2C_FIELD(var_s0, s32 *, 0x1C) == arg1) && (M2C_FIELD(var_s0, f32 *, 0x24) > 0.0f)) {
                func_0020FF80(sp10, arg2 + (M2C_FIELD(M2C_FIELD(var_s0, void **, 8), s32 *, 0xC) << 6));
                func_0021034C(sp10, 0.00000015258789f, 0.00000015258789f, 0.00000015258789f);
                func_0020F7EC((s32) sp10);
                func_002333C0((s32) arg0, (s32) var_s0, (s32) sp10);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 4);
        } while (var_s0 != NULL);
    }
}
