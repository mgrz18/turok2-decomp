#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0027C074();                            /* extern */
M2C_UNK func_002883E0(M2C_UNK *, void *, s32, s32, s32, s32); /* extern */
extern M2C_UNK D_800F6CB0;

void func_00280FD8(void *arg0) {
    f32 temp_f2;

    func_0027C074();
    if (M2C_FIELD(arg0, s32 *, 0x52C) != 0) {
        temp_f2 = M2C_FIELD(arg0, f32 *, 0x290);
        func_002883E0(&D_800F6CB0, arg0 + 0x536, (s32) temp_f2, (s32) M2C_FIELD(arg0, f32 *, 0x294), (s32) (temp_f2 + M2C_FIELD(arg0, f32 *, 0x288)), (s32) (M2C_FIELD(arg0, f32 *, 0x294) + M2C_FIELD(arg0, f32 *, 0x28C)));
    }
}
