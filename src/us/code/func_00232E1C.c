#include "common.h"
#include "m2c_macros.h"

s32 func_002113A4(void);

s32 func_00232E1C(s32 arg0) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;

    if (arg0 == 0) {
        return 0;
    }
    temp_v0 = func_002113A4();
    temp_v1 = arg0 + 1;
    temp_v0_2 = temp_v0 / temp_v1;
    if (temp_v1 == 0) {

    }
    if ((temp_v1 == -1) && (temp_v0_2 == 0x80000000)) {

    }
    return temp_v0 % temp_v1;
}
