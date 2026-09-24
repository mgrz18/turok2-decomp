#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0026EDA8(s32);                         /* extern */

void func_0025359C(void *arg0) {
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    if (var_s0 != NULL) {
        do {
            func_0026EDA8(M2C_FIELD(var_s0, void **, 0xA50) + 0x140);
            M2C_FIELD(M2C_FIELD(var_s0, void **, 0xA50), s32 *, 0xD0) = 0;
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
}
