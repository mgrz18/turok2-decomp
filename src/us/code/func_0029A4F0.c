#include "common.h"

s32 __osSpSetStatus(s32);                     /* extern */

void osSpTaskYield(void) {
    __osSpSetStatus(0x400);
}
