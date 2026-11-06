#include "common.h"
#include "m2c_macros.h"

extern s32 D_801309A4;
extern s32 D_801309B0;

void PakConfirmMenu_InitNoSpace_2(void) {
    D_801309B0 = 1;
    D_801309A4 = 0;
}
