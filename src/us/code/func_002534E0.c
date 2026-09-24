#include "common.h"
#include "m2c_macros.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_00285410(s32);

extern M2C_UNK D_8011ACB0;

void func_002534E0(void *arg0) {
    M2C_UNK *var_a0;
    M2C_UNK *var_a0_2;
    M2C_UNK *var_s0;
    void *var_s1;

    var_s0 = &D_8011ACB0;
    var_s0 = &D_8011ACB0;
    var_s1 = arg0;
    if ((M2C_FIELD(&D_8011ACB0, s32 *, 0x1C) == 0) && (M2C_FIELD(&D_8011ACB0, s32 *, 0x20) == 0)) {
        func_00285410(0);
        if (M2C_FIELD(&D_8011ACB0, s32 *, 0x24) != 0) {
            if (M2C_FIELD(&D_8011ACB0, s32 *, 0x28) == 0) {
                M2C_FIELD(&D_8011ACB0, s32 *, 0x28) = 1;
                var_s0 = M2C_FIELD(var_s1, M2C_UNK **, 0x20);
                var_a0 = var_s0;
                if (var_s0 != NULL) {
                    do {
                        func_0024E5F0((s32) var_a0, (s32) var_s0, 0x12);
                        var_s0 = M2C_FIELD(var_s0, M2C_UNK **, 0x1320);
                        var_a0 = var_s0;
                    } while (var_s0 != NULL);
                }
            }
        } else {
            M2C_FIELD(&D_8011ACB0, s32 *, 0x1C) = 1;
            var_s0 = M2C_FIELD(var_s1, M2C_UNK **, 0x20);
            var_a0_2 = var_s0;
            if (var_s0 != NULL) {
                do {
                    func_0024E5F0((s32) var_a0_2, (s32) var_s0, 0x12);
                    var_s0 = M2C_FIELD(var_s0, M2C_UNK **, 0x1320);
                    var_a0_2 = var_s0;
                } while (var_s0 != NULL);
            }
        }
    }
}
