#include "common.h"
#include "m2c_macros.h"

s32 func_00246918(s32, s32);
s32 func_002600E8(s32, s32);

void func_0026007C(s32 arg0, void *arg1) {
    if (M2C_FIELD(arg1, f32 *, 0xD0) <= 0.0f) {
        func_00246918(arg0, (s32) arg1);
    }
    if (M2C_FIELD(arg1, f32 *, 0x40) >= 150.0f) {
        func_002600E8(arg0, (s32) arg1);
    }
}
