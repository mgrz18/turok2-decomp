#include "common.h"
#include "m2c_macros.h"

extern u8 D_800F7078[];
extern s32 func_002241C0(void *, s32);

s32 func_00268C04(u8 *arg0) {
    if (arg0[0] == 1) {
        return M2C_FIELD(arg0, u16 *, 0xB8);
    }
    return func_002241C0(D_800F7078, M2C_FIELD(arg0, u16 *, 2));
}
