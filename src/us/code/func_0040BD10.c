#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern void *D_8010FEB4;

void func_0040BD10(void) {
    void *var_s0;

    var_s0 = D_8010FEB4;
    if (var_s0 != NULL) {
        do {
            if (M2C_FIELD(var_s0, s32 *, 0x144) > 0) {
                M2C_FIELD(var_s0, s32 *, 0x144) = 0;
                func_00243414((s32) var_s0, (s32) (var_s0 + 0x140), 0x34);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x28C);
        } while (var_s0 != NULL);
    }
}
