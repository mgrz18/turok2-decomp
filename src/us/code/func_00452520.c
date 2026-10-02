#include "common.h"
#include "m2c_macros.h"

s32 func_00416644(s32, s32, s32, s32, s32);

M2C_UNK func_00285A68(s8);                          /* extern */
extern M2C_UNK D_0043BC64;
extern M2C_UNK D_80119E2C;

s32 func_00452520(s32 arg0, void *arg1) {
    func_00285A68(M2C_FIELD(M2C_FIELD(arg1, void **, 0x20), s8 *, 4));
    func_00416644((s32) &D_80119E2C, (s32) &D_0043BC64, M2C_FIELD(arg1, s32 *, 0x1C), (s32) M2C_FIELD(arg1, void **, 0x20), 0);
    return 1;
}
