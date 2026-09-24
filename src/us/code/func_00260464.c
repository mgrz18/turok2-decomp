#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00245BAC(s32, void *, f32, f32);       /* extern */
f32 func_00246310(s32, s32, s32, s32);              /* extern */
extern f32 D_800A798C;
extern f32 D_800A7990;
extern M2C_UNK D_800B06A0;

void func_00260464(s32 arg0, void *arg1) {
    f32 var_f20;

    var_f20 = D_800A798C;
    if ((u32) (M2C_FIELD(arg1, u8 *, 0x36) - 1) < 2U) {
        var_f20 = D_800A7990;
    }
    func_00245BAC(arg0, arg1, var_f20, func_00246310(arg0, M2C_FIELD(&D_800B06A0, s32 *, 0), M2C_FIELD(&D_800B06A0, s32 *, 4), M2C_FIELD(&D_800B06A0, s32 *, 8)));
}
