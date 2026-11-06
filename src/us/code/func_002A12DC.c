#include "common.h"
#include "m2c_macros.h"

extern s32 D_80125C48;

void adecClearStopFlag(void) {
    D_80125C48 = 0;
}
