#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_002241D8(M2C_UNK *, void *, u16, s32); /* extern */
extern M2C_UNK D_800AF618;
extern M2C_UNK D_800F7078;

void func_0025426C(void *arg0) {
    func_002241D8(&D_800F7078, arg0 + 0x288, M2C_FIELD(*(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(arg0, s16 *, 0x996) * 4)))), u16 *, 4), M2C_FIELD(M2C_FIELD(arg0, void **, 0x3F4), s32 *, 0x10));
}
