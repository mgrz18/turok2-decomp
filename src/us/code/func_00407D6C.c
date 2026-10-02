#include "common.h"
#include "m2c_macros.h"

s32 func_00248BA8(s32, s32, s32, s32);

void *func_002532A8(M2C_UNK *);                     /* extern */
s32 func_00284204();                                /* extern */
extern M2C_UNK D_80119870;

void func_00407D6C(void) {
    void *temp_v0;

    if (func_00284204() != 0) {
        temp_v0 = func_002532A8(&D_80119870);
        if (temp_v0 != NULL) {
            M2C_FIELD(temp_v0, s32 *, 0x9F0) = 1;
            func_00248BA8((s32) temp_v0, 0x1F40, 2, 0);
        }
    }
}
