#include "common.h"
#include "m2c_macros.h"

s32 func_00245A98(s32, s32, s32, s32);

extern M2C_UNK D_800AF870;

void func_00259F3C(void *arg0, s32 arg1) {
    func_00245A98((s32) arg0, arg1, (s32) &D_800AF870, (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), u16 *, 0x92));
}
