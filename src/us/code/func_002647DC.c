#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_002647DC(void *arg0, s32 *arg1) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2100);
    *arg1 |= 0x08000000;
    func_00243414((s32) arg0, (s32) arg1, 1);
}
