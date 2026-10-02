#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800C0544;
extern s32 D_8012FA04;

void func_00407800(void *arg0, void *arg1) {
    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        M2C_FIELD(arg0, f32 *, 0x1FC) = (f32) D_800C0544;
        D_8012FA04 = -1;
        func_00243414((s32) arg0, (s32) arg1, 0x13);
    }
}
