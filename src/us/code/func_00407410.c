#include "common.h"
#include "m2c_macros.h"

extern s32 D_8012F9D8;

void func_00407410(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) | 0x2000);
    D_8012F9D8 = 0;
}
