#include "common.h"
#include "m2c_macros.h"

void func_002101A0(void *arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0) = (f32) M2C_FIELD(arg0, f32 *, 0x30);
    M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 0x34);
    M2C_FIELD(arg1, f32 *, 8) = (f32) M2C_FIELD(arg0, f32 *, 0x38);
}
