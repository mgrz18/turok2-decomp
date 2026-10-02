#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_00444324(void *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        M2C_FIELD(arg0, s32 *, 0x27C) = (s32) (M2C_FIELD(arg0, s32 *, 0x27C) & 0xF7FFFFFF);
        func_00243414((s32) arg0, (s32) arg1, 2);
    }
}
