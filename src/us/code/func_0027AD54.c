#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0027AD00(void *, M2C_UNK *);           /* extern */
extern M2C_UNK D_800B2C64;

void func_0027AD54(void *arg0, s32 arg1, s32 arg2) {
    M2C_FIELD(arg0, s32 *, 0x88) = arg1;
    M2C_FIELD(arg0, s32 *, 0x84) = arg2;
    M2C_FIELD(arg0, s32 *, 0x8C) = 0x64;
    func_0027AD00(arg0 + 0x48, &D_800B2C64);
}
