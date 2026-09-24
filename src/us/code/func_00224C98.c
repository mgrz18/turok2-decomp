#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, M2C_UNK);                    /* extern */

void func_00224C98(void *arg0, s32 arg1) {
    s32 temp_v0;
    u8 *temp_v0_2;

    temp_v0 = func_002017D4(M2C_FIELD(arg0, s32 *, 0x6C), 1);
    func_002017D4(temp_v0, 0);
    temp_v0_2 = func_002017D4(temp_v0, 1) + arg1;
    *temp_v0_2 -= 1;
}
