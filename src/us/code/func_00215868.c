#include "common.h"
#include "m2c_macros.h"

extern f32 D_800B6D28;

void func_00215868(void *arg0) {
    M2C_FIELD(arg0, f32 *, 0x174) = (f32) (D_800B6D28 * M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), f32 *, 0x20));
}
