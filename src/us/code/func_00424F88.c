#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B436C;
extern M2C_UNK D_800B437C;
extern s32 D_80130A34;

s32 func_00424F88(void *arg0) {
    if (D_80130A34 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B437C;
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B436C;
    }
    return 1;
}
