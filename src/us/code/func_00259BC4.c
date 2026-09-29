#include "common.h"
#include "m2c_macros.h"

s32 func_00275544(s32, s32, s32, s32, s32, s32);

extern M2C_UNK D_800A7348;
extern M2C_UNK D_800AF618;

void func_00259BC4(void *arg0, void *arg1) {
    void *temp_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x1A8), void **, 0x51C);
    temp_v0 = temp_v1 + 0x114;
    func_00275544(0x1BB, M2C_FIELD(temp_v1, s32 *, 0x114), M2C_FIELD(temp_v0, s32 *, 4), M2C_FIELD(temp_v0, s32 *, 8), (s32) temp_v0, -1);
    M2C_FIELD(arg1, s32 *, 0x40) = 0;
    M2C_FIELD(arg1, f32 *, 0x118) = (f32) (M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(M2C_FIELD(arg0, void **, 0x1A8), s16 *, 0x996) * 4)))), f32 *, 0x18) * M2C_FIELD(&D_800A7348, f32 *, 4));
}
