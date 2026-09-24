#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00200518(void *, void *);              /* extern */
M2C_UNK func_00200738(void *, void *);              /* extern */

void *func_00228AEC(void *arg0, s32 arg1) {
    void *temp_s0;

    temp_s0 = M2C_FIELD(arg0, void **, 0x14);
    if (temp_s0 != NULL) {
        func_00200738(arg0 + 0x14, temp_s0);
        func_00200518(arg0, temp_s0);
        M2C_FIELD(temp_s0, s32 *, 8) = arg1;
        M2C_FIELD(temp_s0, s16 *, 0x16) = 0;
    }
    return temp_s0;
}
