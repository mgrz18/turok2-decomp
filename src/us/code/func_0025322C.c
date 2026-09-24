#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0024C648(void *, s32);                 /* extern */
M2C_UNK func_002812CC(s32);                         /* extern */

void func_0025322C(void *arg0, s32 arg1) {
    void *var_s0;

    func_002812CC(arg1);
    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    if (var_s0 != NULL) {
        do {
            if (M2C_FIELD(var_s0, s32 *, 0x51C) == arg1) {
                func_0024C648(var_s0, arg1);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
}
