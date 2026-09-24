#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275D34(s32, s32);                    /* extern */
s32 func_00275F7C();                                /* extern */

void func_00232B40(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x1B8) != 0) {
        func_00275D34(func_00275F7C(), M2C_FIELD(arg0, s32 *, 0x1B8));
        M2C_FIELD(arg0, s32 *, 0x1B8) = 0;
    }
}
