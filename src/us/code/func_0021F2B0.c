#include "common.h"
#include "m2c_macros.h"

void *func_002017D4(s32, M2C_UNK);                  /* extern */
s32 *func_0020367C(M2C_UNK, s32, s32, M2C_UNK, s32, s32, M2C_UNK *, s32); /* extern */
M2C_UNK func_002051F4(M2C_UNK, s32 *);              /* extern */
f32 func_0026D210(u16);                             /* extern */
extern M2C_UNK D_800A5AC4;
extern f32 D_800A5D18;

f32 func_0021F2B0(void *arg0) {
    f32 var_f0;
    f32 var_f20;
    s32 *temp_v0;

    var_f20 = D_800A5D18;
    if (M2C_FIELD(arg0, s32 *, 0xD4) & 0x40000) {
        temp_v0 = func_0020367C(0, M2C_FIELD(arg0, s32 *, 0xA4), M2C_FIELD(arg0, s32 *, 0xB0), 4, 0, 0, &D_800A5AC4, 1);
        var_f0 = var_f20;
        if (temp_v0 != NULL) {
            var_f20 = func_0026D210(M2C_FIELD(func_002017D4(*temp_v0, 0), u16 *, 0x1E));
            func_002051F4(0, temp_v0);
            goto block_3;
        }
    } else {
block_3:
        var_f0 = var_f20;
    }
    return var_f0;
}
