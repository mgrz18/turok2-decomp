#include "common.h"
#include "m2c_macros.h"

extern u8 D_002972E0[];
extern s32 func_002973E0(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

void func_002954F8(void *arg0) {
    func_00296340(arg0, D_002972E0, func_002973E0, 3);
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = 1;
}
