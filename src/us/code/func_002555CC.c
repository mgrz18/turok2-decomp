#include "common.h"
#include "m2c_macros.h"

s32 func_00267800(s32);
s32 func_00268798(s32, s32);
s32 func_00275544(s32, s32, s32, s32, s32, s32);

M2C_UNK func_0020EEF8(void *, void *, M2C_UNK *);   /* extern */
extern M2C_UNK D_800A7168;
extern f32 D_800A7170;
extern s32 D_80110048;
extern u8 D_8011AAF1;

void func_002555CC(s32 arg0, void *arg1) {
    u8 sp18[0x10];
    f32 var_f1;
    s32 temp_v0;
    void *temp_a0;

    var_f1 = M2C_FIELD(&D_800A7168, f32 *, 4);
    if (D_80110048 == 0x1DB1) {
        var_f1 = D_800A7170;
    }
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) (var_f1 * M2C_FIELD(M2C_FIELD(arg1, void **, 0x14), f32 *, 0x20));
    temp_v0 = func_00267800((s32) arg1);
    if (temp_v0 != 0) {
        func_00268798(temp_v0, (s32) sp18);
        temp_a0 = arg1 + 0x18;
        func_0020EEF8(temp_a0, temp_a0, sp18);
    }
    if (D_8011AAF1 == 0) {
        func_00275544(0x3AC1, M2C_FIELD(arg1, s32 *, 4), M2C_FIELD(arg1, s32 *, 8), M2C_FIELD(arg1, s32 *, 0xC), 0, -1);
    }
}
