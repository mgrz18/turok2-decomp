#include "common.h"
#include "m2c_macros.h"

s32 func_002757E4(s32);                             /* extern */

void func_00275E98(void *arg0) {
    void *var_s0;

    if (M2C_FIELD(arg0, s32 *, 0x28) == 0) {
        var_s0 = M2C_FIELD(arg0, void **, 0x14);
        if (var_s0 != NULL) {
            do {
                if (func_002757E4(M2C_FIELD(var_s0, s32 *, 8)) == 0) {
                    M2C_FIELD(var_s0, s32 *, 0xC) = -1;
                    M2C_FIELD(var_s0, s32 *, 8) = -1;
                }
                var_s0 = M2C_FIELD(var_s0, void **, 4);
            } while (var_s0 != NULL);
        }
    }
}
