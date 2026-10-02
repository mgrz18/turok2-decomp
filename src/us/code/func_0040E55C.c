#include "common.h"
#include "m2c_macros.h"

s32 func_00225C84(s32);

extern M2C_UNK D_800F7078;

void func_0040E55C(void *arg0) {
    void *temp_v0;

    temp_v0 = M2C_FIELD(arg0, void **, 0x4F8);
    M2C_FIELD(arg0, s32 *, 0x500) = (s32) M2C_FIELD(temp_v0, f32 *, 4);
    M2C_FIELD(arg0, s32 *, 0x508) = (s32) M2C_FIELD(temp_v0, f32 *, 8);
    func_00225C84((s32) &D_800F7078);
}
