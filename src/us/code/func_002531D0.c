#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00249898(void *, void *);              /* extern */

void func_002531D0(void *arg0, void *arg1) {
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    if (var_s0 != NULL) {
        do {
            if ((M2C_FIELD(var_s0, s32 *, 0x51C) == arg1) && (M2C_FIELD(arg1, s32 *, 0x24) == 0)) {
                func_00249898(var_s0, arg1);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
}
