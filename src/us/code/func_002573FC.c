#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800AF618;
extern M2C_UNK D_8011AAF1;

void func_002573FC(void *arg0) {
    s16 temp_a1;
    void *temp_v0;

    if ((M2C_FIELD(&D_8011AAF1, u8 *, 0) == 0) && ((M2C_FIELD(arg0, s32 *, 0x1050) == 0) || (M2C_FIELD(arg0, s32 *, 0xCD0) == 0)) && ((M2C_FIELD(&D_8011AAF1, s32 *, -0x51) == 0) || !(M2C_FIELD(arg0, s32 *, 0xA64) & 0x20)) && (M2C_FIELD(arg0, s32 *, 0xA68) & 0x200)) {
        temp_v0 = *(((__typeof__(&D_800AF618))((s8 *)&D_800AF618 + (M2C_FIELD(arg0, s16 *, 0xB14) * 4))));
        temp_a1 = M2C_FIELD(temp_v0, s16 *, 0xC);
        if ((temp_a1 != -1) && (M2C_FIELD((arg0 + temp_a1), s8 *, 0x972) != 0)) {
            M2C_FIELD(arg0, s16 *, 0xB14) = (s16) (u16) M2C_FIELD(temp_v0, s16 *, 0xC);
        }
    }
}
