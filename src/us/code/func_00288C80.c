#include "common.h"
#include "m2c_macros.h"

s32 func_00287ABC(s32);
s32 func_0028D23C(void);

extern s32 D_800B6D14;
extern s32 D_800B6D24;
extern M2C_UNK D_800B6D7C;
extern s32 D_8011B110;

void func_00288C80(void *arg0) {
    M2C_UNK (*temp_v0)(void *);

    if (D_800B6D14 != 0) {
        D_800B6D14 = 0;
    }
    func_0028D23C();
    temp_v0 = *(((__typeof__(&D_800B6D7C))((s8 *)&D_800B6D7C + (M2C_FIELD(arg0, s32 *, 0x23FD8) * 0xC))));
    if (temp_v0 != NULL) {
        temp_v0(arg0);
    }
    if ((M2C_FIELD(arg0, s32 *, 0x23FD8) != 0x11) && (D_8011B110 != 0)) {
        D_8011B110 -= 1;
        func_00287ABC((s32) arg0);
    }
    D_800B6D24 += 1;
}
