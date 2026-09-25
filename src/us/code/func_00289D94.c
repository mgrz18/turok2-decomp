#include "common.h"
#include "m2c_macros.h"

void func_0029AA80(s32);
s32 func_0029AAD0(s32);

M2C_UNK func_0029AC40(f32);                         /* extern */
extern f32 D_800A9E24;
extern M2C_UNK D_800B7BE0;
extern M2C_UNK D_800B7E60;
extern s32 D_800C2038;

void func_00289D94(void *arg0) {
    M2C_UNK *var_a0;

    if (M2C_FIELD(arg0, s32 *, 0x23FF0) != 0) {
        var_a0 = &D_800B7E60;
        if (D_800C2038 == 0) {
            var_a0 = &D_800B7BE0;
        }
        func_0029AA80((s32) var_a0);
        func_0029AC40(D_800A9E24);
        func_0029AAD0(2);
    }
}
