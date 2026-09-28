#include "common.h"
#include "m2c_macros.h"

s32 func_002973E0(void *arg0, s32 arg1, s32 arg2) {
    switch (arg1) {
        case 1:
            M2C_FIELD(arg0, s32 *, 0) = arg2;
            break;
        case 6:
            M2C_FIELD(arg0, s32 *, 0x14) = arg2;
            break;
    }
    return 0;
}
