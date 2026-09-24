#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200574(void *, void *);              /* extern */
M2C_UNK func_00200738(void *, void *);              /* extern */
M2C_UNK func_00275864(s32);                         /* extern */

void func_00275EFC(void *arg0) {
    void *temp_s0;
    void *var_s1;

    var_s1 = M2C_FIELD(arg0, void **, 0x14);
    if (var_s1 != NULL) {
        do {
            temp_s0 = M2C_FIELD(var_s1, void **, 4);
            func_00275864(M2C_FIELD(var_s1, s32 *, 8));
            M2C_FIELD(var_s1, s32 *, 0xC) = -1;
            M2C_FIELD(var_s1, s32 *, 8) = -1;
            func_00200738(arg0 + 0x14, var_s1);
            func_00200574(arg0, var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
