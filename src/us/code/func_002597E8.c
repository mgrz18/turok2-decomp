#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00253DE0(void *, M2C_UNK);             /* extern */
M2C_UNK func_0027588C(M2C_UNK);                     /* extern */
extern s32 D_800B6D20;

void func_002597E8(void *arg0, void *arg1) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x1A8);
    if (M2C_FIELD(temp_a0, s32 *, 0x1288) == 0) {
        if (M2C_FIELD(arg1, s8 *, 0x34) != 1) {
            func_00253DE0(temp_a0, 0x1A8);
        }
    }
    if ((M2C_FIELD(arg1, s8 *, 0x34) != 4) && (D_800B6D20 & 1)) {
        func_0027588C(0x1A9);
    }
}
