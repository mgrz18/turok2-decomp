#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B17C0;
extern M2C_UNK D_800B1840;
extern M2C_UNK D_800B18C0;

s32 func_0025EAB0(void *arg0, void *arg1) {
    s32 temp_v1;

    temp_v1 = M2C_FIELD(arg1, s32 *, 0x2C);
    if ((temp_v1 == &D_800B17C0) || (temp_v1 == &D_800B1840) || (temp_v1 == &D_800B18C0)) {
        return 0;
    }
    return M2C_FIELD(arg0, u16 *, 0xB8) != 0x259;
}
