#include "common.h"
#include "m2c_macros.h"

extern void func_00224F48(void *, s32, s32, s32);
extern u8 D_800F7078[];

void func_00207914(s32 arg0, s32 arg1) {
    func_00224F48(D_800F7078, arg1, arg0, arg0 + 4);
}
