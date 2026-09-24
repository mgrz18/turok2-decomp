#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0029A4F0(void *);                      /* extern */

void func_0028B274(void *arg0) {
    void *temp_a0;

    temp_a0 = M2C_FIELD(arg0, void **, 0x2F4);
    if (M2C_FIELD(temp_a0, s32 *, 0x10) == 1) {
        M2C_FIELD(temp_a0, s32 *, 4) = (s32) (M2C_FIELD(temp_a0, s32 *, 4) | 0x10);
        func_0029A4F0(temp_a0);
    }
}
