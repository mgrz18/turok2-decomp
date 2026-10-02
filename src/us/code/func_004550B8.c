#include "common.h"
#include "m2c_macros.h"

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_8011AB18;

s32 func_004550B8(s32 arg0, void *arg1) {
    M2C_UNK *temp_v0_2;
    M2C_UNK *var_s0;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x1C);
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = M2C_FIELD(temp_v0, M2C_UNK **, 0x518);
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    M2C_FIELD(var_s0, u8 *, 0x2E) = func_0041648C((s32) arg1, (s32) M2C_FIELD(var_s0, u8 *, 0x2E), 1, 0, 1, 1);
    return 0;
}
