#include "common.h"
#include "m2c_macros.h"

s32 func_002548FC(void *arg0) {
    s16 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s16 *, 0xA08);
    if ((temp_v1 == 0x12) || (temp_v1 == 0x10) || (temp_v1 == 0x11)) {
        return 1;
    }
    return 0;
}
