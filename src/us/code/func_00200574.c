#include "common.h"
#include "m2c_macros.h"

void func_00200574(void *arg0, s32 arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg0, s32 *, 4);
    if (temp_v1 != 0) {
        *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8)) = temp_v1;
        *(s32 *)(M2C_FIELD(arg0, s32 *, 4) + M2C_FIELD(arg0, s32 *, 0xC)) = arg1;
    } else {
        *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8)) = 0;
        M2C_FIELD(arg0, s32 *, 0) = arg1;
    }
    *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 0xC)) = 0;
    M2C_FIELD(arg0, s32 *, 4) = arg1;
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
