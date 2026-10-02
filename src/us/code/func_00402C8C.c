#include "common.h"
#include "m2c_macros.h"

s32 func_00225C84(s32);

extern M2C_UNK D_800F7078;

void func_00402C8C(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x2000);
    func_00225C84((s32) &D_800F7078);
}
