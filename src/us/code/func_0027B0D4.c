#include "common.h"

s32 COnScreen__SelectSmallFont();                            /* extern */
extern s32 D_800F56A8;

void COnScreen__SelectFont2(void) {
    COnScreen__SelectSmallFont();
    D_800F56A8 = 2;
}
