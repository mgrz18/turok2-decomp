#include "common.h"
#include "m2c_macros.h"

s32 func_002A31E4();                                /* extern */
M2C_UNK func_002A31F0(s32);                         /* extern */

s32 func_002A12F0(void) {
    s32 temp_v0;

    temp_v0 = func_002A31E4();
    func_002A31F0(temp_v0 & ~1);
    return temp_v0 & 1;
}
