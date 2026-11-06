#include "common.h"
#include "m2c_macros.h"

extern s32 D_800C1BB8;

void Cinema_Disable(void) {
    D_800C1BB8 = 0;
}
