#include "common.h"
#include "m2c_macros.h"

s32 func_0020B624(void);
s32 func_0024972C(s32, s32);
s32 func_002842F8(void);

M2C_UNK func_0020B5D4();                            /* extern */

void func_00253148(void *arg0, void *arg1) {
    void *var_s0;

    func_0020B5D4();
    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    if (var_s0 != NULL) {
        do {
            if ((M2C_FIELD(var_s0, s32 *, 0x51C) != arg1) || (M2C_FIELD(arg1, s32 *, 0x24) == 1) || (func_002842F8() != 0)) {
                func_0024972C((s32) var_s0, (s32) arg1);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
    func_0020B624();
}
