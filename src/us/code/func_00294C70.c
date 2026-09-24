#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029B020(M2C_UNK *, M2C_UNK *, M2C_UNK); /* extern */
extern M2C_UNK D_800AA4B0;
extern M2C_UNK D_800AA4B4;

s32 func_00294C70(void *arg0, void **arg1) {
    s32 var_s0;
    void *var_a0;

    var_s0 = 0x7FFFFFFF;
    if (M2C_FIELD(arg0, void **, 0) == NULL) {
        func_0029B020(&D_800AA4B0, &D_800AA4B4, 0x133);
    }
    *arg1 = NULL;
    var_a0 = M2C_FIELD(arg0, void **, 0);
    if (var_a0 != NULL) {
        do {
            if ((M2C_FIELD(var_a0, s32 *, 0x10) - M2C_FIELD(arg0, s32 *, 0x20)) < var_s0) {
                *arg1 = var_a0;
                var_s0 = M2C_FIELD(var_a0, s32 *, 0x10) - M2C_FIELD(arg0, s32 *, 0x20);
            }
            var_a0 = M2C_FIELD(var_a0, void **, 0);
        } while (var_a0 != NULL);
    }
    return M2C_FIELD(*arg1, s32 *, 0x10);
}
