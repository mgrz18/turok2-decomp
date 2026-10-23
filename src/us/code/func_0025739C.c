#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_8011AAF1;

void func_0025739C(void *arg0) {
    if ((M2C_FIELD(&D_8011AAF1, u8 *, 0) == 0) && ((M2C_FIELD(&D_8011AAF1, s32 *, -0x51) == 0) || !(M2C_FIELD(arg0, s32 *, 0xA64) & 0x20)) && (M2C_FIELD(arg0, s32 *, 0xA68) & 0x800)) {
        if (M2C_FIELD(arg0, s32 *, 0x12A4) != 0) {
            M2C_FIELD(arg0, s32 *, 0x12A4) = 0;
            return;
        }
        M2C_FIELD(arg0, s32 *, 0x12A4) = 1;
    }
}
