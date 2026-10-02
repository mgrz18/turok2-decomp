#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_00439774;
extern M2C_UNK D_800C1144;
extern u32 D_80130920;

s32 func_0041AEB0(void *arg0) {
    if ((u32) D_80130920 < 0xCU) {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = (M2C_UNK *) *(((__typeof__(&D_00439774))((s8 *)&D_00439774 + (D_80130920 * 0x10))));
    } else {
        M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800C1144;
    }
    return 0;
}
