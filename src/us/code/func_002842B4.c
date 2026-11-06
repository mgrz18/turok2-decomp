#include "common.h"
#include "m2c_macros.h"

extern s32 D_800C1BB8;

void Cinema_Enable(void) {
    D_800C1BB8 = 1;
}
