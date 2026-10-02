#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002666B0(s32, s32, M2C_UNK);           /* extern */

void func_0040340C(void *arg0, M2C_UNK arg1) {
    s32 temp_a0;

    temp_a0 = M2C_FIELD(arg0, s32 *, 0x220);
    func_002666B0(temp_a0, temp_a0 + 0x140, arg1);
}
