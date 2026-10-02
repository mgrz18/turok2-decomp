#include "common.h"
#include "m2c_macros.h"

s32 func_00416644(s32, s32, s32, s32, s32);

extern M2C_UNK D_00438728;

s32 func_00451320(s32 arg0, void *arg1) {
    void *temp_a2;

    temp_a2 = M2C_FIELD(arg1, void **, 0x1C);
    M2C_FIELD(temp_a2, s32 *, 0xBF4) = 0;
    M2C_FIELD(M2C_FIELD(temp_a2, void **, 0x518), s8 *, 0x30) = 0;
    func_00416644(M2C_FIELD(temp_a2, s32 *, 0x51C) + 0x53C, (s32) &D_00438728, (s32) temp_a2, M2C_FIELD(temp_a2, s32 *, 0xA50), M2C_FIELD(temp_a2, s32 *, 0x514));
    return 1;
}
