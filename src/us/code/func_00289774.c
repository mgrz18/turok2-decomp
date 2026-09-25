#include "common.h"
#include "m2c_macros.h"

extern void func_00288DD0(s32, s32);
extern s32 D_800B6D60;

void func_00289774(s32 arg0) {
    D_800B6D60 += 1;
    func_00288DD0(arg0, 0x6F);
}
