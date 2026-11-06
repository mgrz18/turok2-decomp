#include "common.h"
#include "globals/menu.h"

s32 COnScreen__SelectSmallFont();                            /* extern */

void COnScreen__SelectFont2(void) {
    COnScreen__SelectSmallFont();
    gLoadedFont = 2;
}
