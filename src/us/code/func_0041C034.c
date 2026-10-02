#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B3F00;
extern M2C_UNK D_800B3F18;
extern M2C_UNK D_8011AB18;

s32 func_0041C034(void *arg0, void *arg1) {
    M2C_UNK *temp_v0_2;
    M2C_UNK *var_v1;
    u8 temp_v1;
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg1, void **, 0x1C);
    var_v1 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = M2C_FIELD(temp_v0, M2C_UNK **, 0x518);
        if (temp_v0_2 != NULL) {
            var_v1 = temp_v0_2;
        }
    }
    temp_v1 = M2C_FIELD(var_v1, u8 *, 0x2C);
    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B3F00;
        }
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B3F18;
    }
    return 0;
}
