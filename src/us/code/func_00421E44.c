#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B49C4;
extern M2C_UNK D_800B49D0;
extern s32 D_801309B8;

s32 func_00421E44(void *arg0) {
    if (D_801309B8 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B49C4;
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B49D0;
    }
    return 0;
}
