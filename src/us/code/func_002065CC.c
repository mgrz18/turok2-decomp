#include "common.h"
#include "m2c_macros.h"

extern s32 func_00200818(s32, s32);
extern s32 D_800D8DF0[];

void func_002065CC(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 *, 0x10) = D_800D8DF0[0];
    func_00200818((s32)((s8 *)D_800D8DF0 - 0xC10), (s32) arg1);
}
