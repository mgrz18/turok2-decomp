#include "common.h"

s32 COnScreen__SelectSmallFont();                            /* extern */
extern s32 gLoadedFont;

void COnScreen__SelectFont2(void) {
    COnScreen__SelectSmallFont();
    gLoadedFont = 2;
}
