#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002051F4(M2C_UNK, s32);                /* extern */

void func_00219938(void *arg0) {
    func_002051F4(0, M2C_FIELD(arg0, s32 *, 0x10));
    M2C_FIELD(arg0, s32 *, 0x10) = 0;
}
