#include "common.h"
#include "m2c_macros.h"

void func_0040E0E0(void *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & ~0x100);
    }
}
