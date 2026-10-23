#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A723C;
extern f32 D_800A7240;

void func_002576AC(void *arg0) {
    f32 temp_f1;
    s32 var_a1;

    var_a1 = 0;
    if (M2C_FIELD(arg0, s32 *, 0xC04) == 0x1144) {
        var_a1 = M2C_FIELD(arg0, s8 *, 0xE2) == 0;
    }
    if (var_a1 == 0) {
        temp_f1 = M2C_FIELD(arg0, f32 *, 0xA78);
        if (D_800A723C <= temp_f1) {
            M2C_FIELD(arg0, s32 *, 0xC04) = 0x8A2;
            return;
        }
        if (temp_f1 <= D_800A7240) {
            M2C_FIELD(arg0, s32 *, 0xC04) = 0x8A7;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0xC04) = 0x14;
    }
}
