#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);
s32 func_0026DFB0(s32);

extern f32 D_800B6D28;

void func_002253F8(void *arg0) {
    f32 temp_f1;
    s32 var_s1;
    s32 var_v1;
    void *temp_a0;
    void *temp_s2;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x1180);
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            temp_a0 = M2C_FIELD(var_s0, void **, 8);
            temp_s2 = M2C_FIELD(var_s0, void **, 4);
            var_v1 = var_s1;
            if (M2C_FIELD(temp_a0, u8 *, 0xE) & 2) {
                temp_f1 = M2C_FIELD(var_s0, f32 *, 0xC) - D_800B6D28;
                M2C_FIELD(var_s0, f32 *, 0xC) = temp_f1;
                if (temp_f1 <= 0.0f) {
                    var_v1 = 1;
                    var_s1 = 1;
                }
            }
            if (var_v1 != 0) {
                func_0026DFB0((s32) temp_a0);
            }
            if (var_s1 != 0) {
                func_00200738((s32) (arg0 + 0x1180), (s32) var_s0);
                func_00200518((s32) (arg0 + 0x1194), (s32) var_s0);
            }
            var_s0 = temp_s2;
            var_s1 = 0;
        } while (var_s0 != NULL);
    }
}
