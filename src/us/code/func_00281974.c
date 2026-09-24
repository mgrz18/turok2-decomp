#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200518(s32, M2C_UNK *);              /* extern */
M2C_UNK func_00200738(void *, M2C_UNK *);           /* extern */

void func_00281974(s32 arg0, void *arg1) {
    M2C_UNK *temp_s0;
    M2C_UNK *var_s1;
    void *var_a0;

    var_s1 = M2C_FIELD(arg1, M2C_UNK **, 0xE2C);
    if (var_s1 != NULL) {
        var_a0 = arg1 + 0xE28;
        do {
            temp_s0 = *var_s1;
            func_00200738(var_a0, var_s1);
            func_00200518(arg0 + 0xF0C, var_s1);
            var_s1 = temp_s0;
            var_a0 = arg1 + 0xE28;
        } while (var_s1 != NULL);
    }
}
