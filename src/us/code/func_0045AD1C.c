#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B490C;
extern s32 D_80130990;

s32 func_0045AD1C(void *arg0) {
    if (D_80130990 != 0) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800B490C;
    }
    return 0;
}
