#include "common.h"
#include "m2c_macros.h"

void *func_002017D4(s32, M2C_UNK);                  /* extern */
M2C_UNK func_002097AC(s32, f32, void *);            /* extern */
extern s32 D_800B6D18;

void func_0020B4F0(s32 *arg0, f32 arg1) {
    f32 temp_f0;
    s32 temp_s0;
    void *temp_v0;

    temp_s0 = *arg0;
    temp_v0 = func_002017D4(temp_s0, 3);
    if (M2C_FIELD(temp_v0, s32 *, 0) != D_800B6D18) {
        temp_f0 = M2C_FIELD(temp_v0, f32 *, 4) + arg1;
        M2C_FIELD(temp_v0, f32 *, 4) = temp_f0;
        func_002097AC(temp_s0, temp_f0, temp_v0);
    }
}
