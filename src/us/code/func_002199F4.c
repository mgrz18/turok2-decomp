#include "common.h"
#include "m2c_macros.h"

s32 func_002017D4(s32, s32);

s32 func_0021793C();                                /* extern */

s32 func_002199F4(void) {
    return M2C_FIELD(func_002017D4(func_0021793C(), 4), s32 *, 4);
}
