#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);
s32 func_0026DFB0(s32);

void func_0046588C(void *arg0) {
    void *temp_s0;
    void *var_s1;

    var_s1 = M2C_FIELD(arg0, void **, 0x1180);
    if (var_s1 != NULL) {
        do {
            temp_s0 = M2C_FIELD(var_s1, void **, 4);
            func_0026DFB0(M2C_FIELD(var_s1, s32 *, 8));
            func_00200738((s32) (arg0 + 0x1180), (s32) var_s1);
            func_00200518((s32) (arg0 + 0x1194), (s32) var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
