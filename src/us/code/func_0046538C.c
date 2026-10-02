#include "common.h"
#include "m2c_macros.h"

s32 func_00201DBC(s32, s32, s32, s32);

s32 func_0028D0E0();                                /* extern */
extern M2C_UNK D_800D1CD8;
extern M2C_UNK D_800E6AC0;

void func_0046538C(void *arg0) {
    if (func_0028D0E0() == 0) {
        func_00201DBC((s32) &D_800D1CD8, M2C_FIELD(arg0, s32 *, 0x2C), M2C_FIELD(arg0, s32 *, 0x34), (s32) &D_800E6AC0);
    }
}
