#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A7294;
extern M2C_UNK D_800AF618;

void func_00258C20(void *arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0x118) = (f32) (M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(M2C_FIELD(arg0, void **, 0x1A8), s16 *, 0x996) * 4)))), f32 *, 0x18) * D_800A7294);
}
