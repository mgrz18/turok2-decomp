#include "common.h"
#include "m2c_macros.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_00255714(s32, s32);

extern M2C_UNK D_800A7170;
extern s32 D_80110048;

void func_00255670(void *arg0, void *arg1) {
    f32 var_f2;

    var_f2 = M2C_FIELD(&D_800A7170, f32 *, 4);
    if (D_80110048 == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(M2C_FIELD(arg0, s32 *, 0xA64) & 0x10) || (M2C_FIELD(arg0, f32 *, 0xA10) >= 3.0f)) {
        func_0024E5F0((s32) arg0, (s32) arg1, 5);
    } else {
        M2C_FIELD(arg1, f32 *, 0x1C) = (f32) (var_f2 * M2C_FIELD(M2C_FIELD(arg1, void **, 0x14), f32 *, 0x20));
    }
    func_00255714((s32) arg0, (s32) arg1);
}
