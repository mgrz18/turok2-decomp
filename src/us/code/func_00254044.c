#include "common.h"
#include "m2c_macros.h"

s32 func_00225EBC(s32, s32, s32);

extern M2C_UNK D_800F7078;

void func_00254044(void *arg0) {
    M2C_UNK *var_s1;
    s32 var_a2;
    s32 var_v0;
    void *var_s0;

    var_s0 = arg0;
    var_a2 = M2C_FIELD(var_s0, s32 *, 0x520);
    if (M2C_FIELD(M2C_FIELD(var_s0, void **, 0x518), u8 *, 0x3E) == 1) {
        var_a2 = 0xB;
    }
    var_s1 = &D_800F7078;
    var_s1 = &D_800F7078;
    var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, var_a2);
    if (var_v0 == 0) {
        var_v0 = func_00225EBC((s32) &D_800F7078, 0xB, -1);
        if (var_v0 == 0) {
            var_v0 = func_00225EBC((s32) &D_800F7078, -1, -1);
        }
    }
    M2C_FIELD(var_s0, s32 *, 0x14) = var_v0;
    M2C_FIELD(var_s0, f32 *, 0x34) = (f32) M2C_FIELD(var_v0, f32 *, 0x38);
    M2C_FIELD(var_s0, f32 *, 0x38) = (f32) M2C_FIELD(var_v0, f32 *, 0x3C);
    M2C_FIELD(var_s0, f32 *, 0x3C) = (f32) M2C_FIELD(var_v0, f32 *, 0x40);
}
