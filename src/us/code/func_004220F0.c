#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B4B84;
extern M2C_UNK D_800B4B8C;
extern s32 D_801309B8;

s32 func_004220F0(void *arg0) {
    if (D_801309B8 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B4B84;
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B4B8C;
    }
    return 0;
}
